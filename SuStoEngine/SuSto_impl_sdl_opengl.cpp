#include "SuStoUI.h"
#include "SuSto_impl_sdl_opengl.h"
#include "SuSto_event_system.h"
//#include "ModuleRenderer3D.h"

#include "SDL\include\SDL.h"
#include "SDL\include\SDL_syswm.h"
#include "SDL\include\SDL_opengl.h"

#include "UICanvas.h"

SuStoUIMain* SuStoUI::Init(SDL_Window* window)
{
	SuStoUIMain* ui_main = new SuStoUIMain();

	ui_main->Awake();
	ui_main->Start();

	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG_OUTPUT("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
	}

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		LOG_OUTPUT("Error on SDL_Init_Video");
	}

	if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0)
	{
		LOG_OUTPUT("Error on SDL_Init_GameController");
	}

	ui_main->GetEventSystem()->CharToKey = SuStoUI::CharToKey;
	ui_main->GetEventSystem()->KeyToChar = SuStoUI::KeyToChar;

	return ui_main;
}

void SuStoUI::NewFrame(SuStoUIMain* ui_main, SDL_Window* window, SuStoVec2 viewport)
{
	// Event
	SuStoUI::EventNewFrame(ui_main);

	// Viewport 
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	ui_main->SetWindowViewport(SuStoVec2(w, h));

	ui_main->SetViewport(viewport);

	ui_main->Update();
}

void SuStoUI::Render(SuStoUIMain * ui_main)
{
	SuStoVec2 window_viewport = ui_main->GetWindowViewport();
	SuStoVec2 viewport = ui_main->GetViewport();

	// 
	GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
	GLfloat last_projection[16]; glGetFloatv(GL_PROJECTION_MATRIX, &last_projection[0]);
	//

	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, window_viewport.x, window_viewport.y);

	//Draw -------------

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	std::vector<PrintableElement*> draw = ui_main->GetDrawList();

	for (std::vector<PrintableElement*>::iterator it = draw.begin(); it != draw.end(); ++it)
	{
		switch ((*it)->GetRenderMode())
		{
			case UICanvasRenderMode::CAMERA_SPACE:
			{
				if ((*it)->GetShowOnCamera() || ui_main->GetUIMode() == UIMode::UI_PLAY)
				{
					glMatrixMode(GL_PROJECTION);
					glPushMatrix();
					glLoadIdentity();
					glOrtho(0, viewport.x, viewport.y, 0, -1000.0f, +1000.0f);

					glMatrixMode(GL_MODELVIEW);
					glPushMatrix();
					glLoadIdentity();
					glMultMatrixf((*it)->GetOrtoTransform().Transposed().ptr());

					Draw((*it)->GetVertices(), (*it)->GetNumIndices(), (*it)->GetIndices(), (*it)->GetUvs(), (*it)->GetTextureId());

					glMatrixMode(GL_PROJECTION);
					glPopMatrix();

					glMatrixMode(GL_MODELVIEW);
					glPopMatrix();
				}

				if (ui_main->GetUIMode() == UIMode::UI_EDIT)
				{
					glMatrixMode(GL_MODELVIEW);
					glPushMatrix();
					glMultMatrixf((*it)->GetTransform().Transposed().ptr());

					Draw((*it)->GetVertices(), (*it)->GetNumIndices(), (*it)->GetIndices(), (*it)->GetUvs(), (*it)->GetTextureId());
				}
			}
			break;
			case UICanvasRenderMode::WORLD_SPACE:
			{
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glMultMatrixf((*it)->GetTransform().Transposed().ptr());

				Draw((*it)->GetVertices(), (*it)->GetNumIndices(), (*it)->GetIndices(), (*it)->GetUvs(), (*it)->GetTextureId());
			}
			break;
		}

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	//------------------

	//Restore modified state
	glPolygonMode(GL_FRONT, last_polygon_mode[0]); glPolygonMode(GL_BACK, last_polygon_mode[1]);
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

void SuStoUI::EndFrame(SuStoUIMain* ui_main)
{
}

void SuStoUI::Draw(float * vertices, uint num_indices, uint * indices, float * uvs, uint texture_id)
{
	glVertexPointer(3, GL_FLOAT, sizeof(float) * 3, vertices);
	glTexCoordPointer(3, GL_FLOAT, sizeof(float) * 3, uvs);

	glBindTexture(GL_TEXTURE_2D, texture_id);

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, indices);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void SuStoUI::DeInit(SuStoUIMain * ui_main)
{
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	RELEASE(ui_main);
}

void SuStoUI::EventNewFrame(SuStoUIMain* ui_main)
{
	UIEventSystem* event_system = ui_main->GetEventSystem();
	UIKey* keyboard = event_system->keyboard;
	UIKeyEvent* mouse_buttons = event_system->mouse_buttons;

	SDL_PumpEvents();
	SDL_StartTextInput();

	event_system->ResetInput();

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	// Keys
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i].state == UIKeyEvent::UI_KEY_IDLE)
			{
				keyboard[i].state = UIKeyEvent::UI_KEY_DOWN;
				event_system->AddKeyDown(keyboard[i]);
			}
			else
			{
				keyboard[i].state = UIKeyEvent::UI_KEY_REPEAT;
				event_system->AddKeyRepeat(keyboard[i]);
			}
		}
		else
		{
			if (keyboard[i].state == UIKeyEvent::UI_KEY_REPEAT || keyboard[i].state == UIKeyEvent::UI_KEY_DOWN)
			{
				keyboard[i].state = UIKeyEvent::UI_KEY_UP;
				event_system->AddKeyUp(keyboard[i]);
			}
			else
			{
				keyboard[i].state = UIKeyEvent::UI_KEY_IDLE;
			}
		}
	}

	// Mouse
	int mouse_x = 0;
	int mouse_y = 0;

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	event_system->SetMouseX(mouse_x / SCREEN_SIZE);
	event_system->SetMouseY(mouse_y / SCREEN_SIZE);
	event_system->SetMouseWheel(0);
	event_system->SetMouseXMotion(0);
	event_system->SetMouseYMotion(0);

	for (int i = 0; i < 5; ++i)
	{
		if (buttons & SDL_BUTTON(i))
		{
			if (mouse_buttons[i] == UIKeyEvent::UI_KEY_IDLE)
				mouse_buttons[i] = UIKeyEvent::UI_KEY_DOWN;
			else
				mouse_buttons[i] = UIKeyEvent::UI_KEY_REPEAT;
		}
		else
		{
			if (mouse_buttons[i] == UIKeyEvent::UI_KEY_REPEAT || mouse_buttons[i] == UIKeyEvent::UI_KEY_DOWN)
				mouse_buttons[i] = UIKeyEvent::UI_KEY_UP;
			else
				mouse_buttons[i] = UIKeyEvent::UI_KEY_IDLE;
		}
	}

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_MOUSEWHEEL:
			event_system->SetMouseWheel(e.wheel.y);
			break;

		case SDL_MOUSEMOTION:
			event_system->SetMouseX(e.motion.x / SCREEN_SIZE);
			event_system->SetMouseY(e.motion.y / SCREEN_SIZE);

			event_system->SetMouseXMotion(e.motion.xrel / SCREEN_SIZE);
			event_system->SetMouseYMotion(e.motion.yrel / SCREEN_SIZE);
			break;

		case SDL_TEXTINPUT:
			event_system->text_input.insert(event_system->text_input.size(), e.text.text);
			break;
		}
	}
}

int SuStoUI::CharToKey(const char * key)
{
	return SDL_GetScancodeFromKey(SDL_GetKeyFromName(key));
}

const char * SuStoUI::KeyToChar(int key)
{
	return SDL_GetScancodeName((SDL_Scancode)key);
}

