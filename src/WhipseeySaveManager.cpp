/**
 * @file WhipseeySaveManager.cpp
 * @brief this is the main file for WhispeeySaveManager, tying all functionality together
 */

#pragma warning(push) //disable warnings for raygui only
#pragma warning(disable: 4244 4838 4267 4996)

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include <raygui.h>

#pragma warning(pop)

int main()
{
	int screenWidth = 800;
	int screenHeight = 600;
	
	SetConfigFlags(FLAG_WINDOW_UNDECORATED);
	InitWindow(screenWidth, screenHeight, "raygui - portable window");

	Vector2 mousePosition{ 0 };
	Vector2 windowPosition = GetWindowPosition();//{ 500, 200 };
	Vector2 panOffset = mousePosition;
	bool dragWindow = false;
	
	// SetWindowPosition(windowPosition.x, windowPosition.y);
	
	bool exitWindow = false;
	
	SetTargetFPS(30);

	while (!exitWindow && !WindowShouldClose())
	{
		mousePosition = GetMousePosition();
		
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			if (CheckCollisionPointRec(mousePosition, Rectangle{ 0, 0, (float)screenWidth, 20 }))
			{
				dragWindow = true;
				panOffset = mousePosition;
			}
		}

		if (dragWindow)
		{			
			windowPosition.x += (mousePosition.x - panOffset.x);
			windowPosition.y += (mousePosition.y - panOffset.y);
			
			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) dragWindow = false;

			SetWindowPosition(windowPosition.x, windowPosition.y);
		}

		BeginDrawing();

			ClearBackground(RAYWHITE);

			exitWindow = GuiWindowBox(Rectangle{ 0, 0, (float)screenWidth, (float)screenHeight }, "PORTABLE WINDOW");
			 ;
			DrawText(FormatText("Mouse Position: [ %.0f, %.0f ]", mousePosition.x, mousePosition.y), 10, 40, 10, DARKGRAY);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
