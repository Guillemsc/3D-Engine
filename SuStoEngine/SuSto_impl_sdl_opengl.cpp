#include "SuStoUI.h"
#include "SuSto_impl_sdl_opengl.h"

//#include "ModuleRenderer3D.h"

#include "SDL\include\SDL.h"
#include "SDL\include\SDL_syswm.h"
#include "SDL\include\SDL_opengl.h"

#include "UICanvas.h"

void SuStoUI::Init(SDL_Window* window, SuStoUIMain* ui_main)
{

}

void SuStoUI::NewFrame(SuStoUIMain* ui_main, SDL_Window* window, SuStoVec2 viewport)
{
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

	glViewport(0, 0, window_viewport.x, window_viewport.y);

	//glMultMatrixf(trans.Transposed().ptr());

	//Draw -------------

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	std::vector<PrintableElement*> draw = ui_main->GetDrawList();

	for (std::vector<PrintableElement*>::iterator it = draw.begin(); it != draw.end(); ++it)
	{
		switch ((*it)->GetOwner()->GetCanvas()->GetRenderMode())
		{
			case UICanvasRenderMode::CAMERA_SPACE:
			{
				if ((*it)->GetOwner()->GetCanvas()->GetShowOnCamera())
				{
					glMatrixMode(GL_PROJECTION);
					glPushMatrix();
					glLoadIdentity();
					glOrtho(0, viewport.x, viewport.y, 0, -1.0f, +1.0f);

					glMatrixMode(GL_MODELVIEW);
					glPushMatrix();
					glLoadIdentity();
					glMultMatrixf((*it)->GetOrtoTransform().ptr());

					Draw((*it)->GetVertices(), (*it)->GetNumIndices(), (*it)->GetIndices(), (*it)->GetUvs(), (*it)->GetTextureId());

					glMatrixMode(GL_PROJECTION);
					glPopMatrix();

					glMatrixMode(GL_MODELVIEW);
					glPopMatrix();
				}

				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glMultMatrixf((*it)->GetTransform().ptr());

				Draw((*it)->GetVertices(), (*it)->GetNumIndices(), (*it)->GetIndices(), (*it)->GetUvs(), (*it)->GetTextureId());
			}
			break;
			case UICanvasRenderMode::WORLD_SPACE:
			{
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glMultMatrixf((*it)->GetTransform().ptr());

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

