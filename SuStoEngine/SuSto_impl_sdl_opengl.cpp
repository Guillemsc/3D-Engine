#include "SuStoUI.h"
#include "SuSto_impl_sdl_opengl.h"

//#include "ModuleRenderer3D.h"

#include "SDL\include\SDL.h"
#include "SDL\include\SDL_syswm.h"
#include "SDL\include\SDL_opengl.h"

#include "App.h"

#include "GeometryMath.h"

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

void SuStoUI::Render(SuStoUIMain * ui_main, float3 camera_pos, bool ortographic)
{
	// 
	GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
	GLfloat last_projection[16]; glGetFloatv(GL_PROJECTION_MATRIX, &last_projection[0]);
	//

	float4x4 trans = float4x4::FromTRS(float3(0, 0, 0), Quat::identity, float3(100, 1, 100));

	// Setup 
	if (1)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, ui_main->GetViewport().x, ui_main->GetViewport().y, 0.0f, -10000.0f, +10000.0f);
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(trans.Transposed().ptr());

	//Draw -------------

	std::vector<SuStoPlane*> draw = ui_main->GetDrawList();

	for (std::vector<SuStoPlane*>::iterator it = draw.begin(); it != draw.end(); ++it)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, sizeof(float) * 3, (*it)->GetVertices());
		glTexCoordPointer(3, GL_FLOAT, sizeof(float) * 3, (*it)->GetUvs());

		//glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);

		glDrawElements(GL_TRIANGLES, (*it)->GetNumIndices(), GL_UNSIGNED_INT, (*it)->GetIndices());

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glLoadMatrixf(last_projection);

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		// ---------------------------
		
		//if ((*it)->GetVerticesId() == 0 && (*it)->GetVertices() != nullptr)
		//	(*it)->SetVerticesId(App->renderer3D->LoadBuffer((*it)->GetVertices(), (*it)->GetNumVertices() * 3));

		//if ((*it)->GetIndicesId() == 0 && (*it)->GetIndices() != nullptr)
		//	(*it)->SetIndicesId(App->renderer3D->LoadBuffer((*it)->GetIndices(), (*it)->GetNumIndices()));

		//if ((*it)->GetUvsId() == 0 && (*it)->GetUvs() != nullptr)
		//	(*it)->SetUvsId(App->renderer3D->LoadBuffer((*it)->GetUvs(), (*it)->GetNumUvs() * 3));

		//glEnableClientState(GL_VERTEX_ARRAY);

		//if ((*it)->GetVerticesId() != 0)
		//{
		//	glBindBuffer(GL_ARRAY_BUFFER, (*it)->GetVerticesId());
		//	glVertexPointer(3, GL_FLOAT, 0, NULL);

		//	if ((*it)->GetUvsId() != 0)
		//	{
		//		// UV
		//		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//		glBindBuffer(GL_ARRAY_BUFFER, (*it)->GetUvsId());
		//		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		//	}

		//	if ((*it)->GetIndicesId() != 0)
		//	{
		//		// Index
		//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*it)->GetIndicesId());

		//		// Draw
		//		glDrawElements((GLenum)GL_TRIANGLES, (*it)->GetNumIndices(), GL_UNSIGNED_INT, NULL);
		//	}

		//	// Disable
		//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//	glDisableClientState(GL_VERTEX_ARRAY);

		//	glBindBuffer(GL_ARRAY_BUFFER, 0);
		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//}

		//// Reset
		//glBindTexture(GL_TEXTURE_2D, 0);

		//// Pop matrix
		//glPopMatrix();
	}

	//------------------

	//Restore modified state
	glPolygonMode(GL_FRONT, last_polygon_mode[0]); glPolygonMode(GL_BACK, last_polygon_mode[1]);
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

void SuStoUI::EndFrame(SuStoUIMain* ui_main)
{
}

