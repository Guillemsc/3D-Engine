#include "SuStoUI.h"
#include "SuSto_impl_sdl_opengl.h"

#include "SDL\include\SDL.h"
#include "SDL\include\SDL_syswm.h"
#include "SDL\include\SDL_opengl.h"

void SuStoUI::Init(SDL_Window* window, SuStoUIMain* ui_main)
{

}

void SuStoUI::NewFrame(SDL_Window* window, SuStoUIMain* ui_main)
{
	// Viewport 
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	ui_main->SetViewport(SuStoVec2(w, h));
}

void SuStoUI::Render(SuStoUIMain * ui_main)
{

	// Get last
	GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
	GLint last_matrix_mode; glGetIntegerv(GL_MATRIX_MODE, &last_matrix_mode);

	// Setup 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//glViewport(0, 0, (GLsizei)ui_main->GetViewport().x, (GLsizei)ui_main->GetViewport().y);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//Draw -------------

	std::vector<SuStoPlane*> draw = ui_main->GetDrawList();

	for (std::vector<SuStoPlane*>::iterator it = draw.begin(); it != draw.end(); ++it)
	{
		glVertexPointer(3, GL_FLOAT, sizeof(float), (*it)->GetVertices());
		glTexCoordPointer(3, GL_FLOAT, sizeof(uint), (*it)->GetUvs());

		glDrawElements(GL_TRIANGLES, (*it)->GetNumIndices(), GL_UNSIGNED_INT, (*it)->GetIndices());
	}

	//------------------

	//Restore modified state
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindTexture(GL_TEXTURE_2D, (GLuint)0);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(last_matrix_mode);
	glPopMatrix();
	glPopAttrib();
	glPolygonMode(GL_FRONT, last_polygon_mode[0]); glPolygonMode(GL_BACK, last_polygon_mode[1]);
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

void SuStoUI::EndFrame(SuStoUIMain* ui_main)
{
}

