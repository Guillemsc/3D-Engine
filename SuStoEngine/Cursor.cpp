#include "Cursor.h"

Cursor::Cursor()
{
	Arrow();
}

Cursor::~Cursor()
{
}

void Cursor::Arrow()
{
	if (arrow == nullptr)
		arrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);

	SDL_SetCursor(arrow);

	SDL_ShowCursor(SDL_ENABLE);
}

void Cursor::Ibeam()
{
	if (ibeam == nullptr)
		ibeam = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);

	SDL_SetCursor(ibeam);

	SDL_ShowCursor(SDL_ENABLE);
}

void Cursor::Wait()
{
	if (wait == nullptr)
		wait = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);

	SDL_SetCursor(wait);

	SDL_ShowCursor(SDL_ENABLE);
}

void Cursor::Crosshair()
{
	if (crosshair == nullptr)
		crosshair = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);

	SDL_SetCursor(crosshair);

	SDL_ShowCursor(SDL_ENABLE);
}

void Cursor::SizeNWSE()
{
	if (sizenwse == nullptr)
		sizenwse = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);

	SDL_SetCursor(sizenwse);

	SDL_ShowCursor(SDL_ENABLE);
}

void Cursor::SizeNESW()
{
	if (sizenesw == nullptr)
		sizenesw = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);

	SDL_SetCursor(sizenesw);

	SDL_ShowCursor(SDL_ENABLE);
}

void Cursor::SizeWE()
{
	if (sizewe == nullptr)
		sizewe = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);

	SDL_SetCursor(sizewe);

	SDL_ShowCursor(SDL_ENABLE);
}

void Cursor::SizeNS()
{
	if (sizens == nullptr)
		sizens = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);

	SDL_SetCursor(sizens);

	SDL_ShowCursor(SDL_ENABLE);
}

void Cursor::SizeAll()
{
	if (sizeall == nullptr)
		sizeall = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);

	SDL_SetCursor(sizeall);

	SDL_ShowCursor(SDL_ENABLE);
}

void Cursor::Hand()
{
	if (hand == nullptr)
		hand = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

	SDL_SetCursor(hand);

	SDL_ShowCursor(SDL_ENABLE);
}

void Cursor::No()
{
	SDL_ShowCursor(SDL_DISABLE);
}

void Cursor::CleanUp()
{
	if (arrow != nullptr)
		SDL_FreeCursor(arrow);

	if (ibeam == nullptr)
		SDL_FreeCursor(ibeam);

	if (wait == nullptr)
		SDL_FreeCursor(wait);

	if (crosshair == nullptr)
		SDL_FreeCursor(crosshair);

	if (sizenwse == nullptr)
		SDL_FreeCursor(sizenwse);

	if (sizenesw == nullptr)
		SDL_FreeCursor(sizenesw);

	if (sizewe == nullptr)
		SDL_FreeCursor(sizewe);

	if (sizens == nullptr)
		SDL_FreeCursor(sizens);

	if (sizeall == nullptr)
		SDL_FreeCursor(sizeall);

	if (hand == nullptr)
		SDL_FreeCursor(hand);
}