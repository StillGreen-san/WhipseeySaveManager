/**
 * @file WhipseeySaveManager.cpp
 * @brief this is the main file for WhispeeySaveManager, tying all functionality together
 */

#pragma warning(push) //disable warnings for raygui only
#pragma warning(disable: 4244 4838 4267 4996)

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_RICONS
#include <raygui.h>

#pragma warning(pop)

#include <string>

//TODO no c cast

int main()
{
	std::string windowTitle = "Whipseey Save Manager";
	std::string input; input.resize(60);
	
	int screenWidth = 800;
	int screenHeight = 600;
	
	SetConfigFlags(FLAG_WINDOW_UNDECORATED);
	InitWindow(screenWidth, screenHeight, windowTitle.c_str());

	Vector2 mousePosition;
	Vector2 windowPosition = GetWindowPosition();
	Vector2 panOffset;
	bool dragWindow = false;
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

			SetWindowPosition((int)windowPosition.x, (int)windowPosition.y);
		}

		BeginDrawing();

			ClearBackground(RAYWHITE);

			exitWindow = GuiWindowBox(Rectangle{ 0, 0, (float)screenWidth, (float)screenHeight }, windowTitle.c_str());

			GuiTextBox(Rectangle{10, 30, 529, 20}, input.data(), static_cast<int>(input.size()), false);
			GuiButton(Rectangle{545, 30, 40, 20}, "#1#");
			GuiButton(Rectangle{590, 30, 45, 20}, "save");
			GuiButton(Rectangle{640, 30, 50, 20}, "reload");

			GuiGroupBox(Rectangle{10, 60, 220, 175}, "File1  [ 63-69420 ]");
			GuiGroupBox(Rectangle{20, 78, 90, 145}, "Level Progress");
			GuiCheckBox(Rectangle{40, 95, 15, 15}, "Castle", false);
			GuiCheckBox(Rectangle{40, 115, 15, 15}, "Moon", false);
			GuiCheckBox(Rectangle{40, 135, 15, 15}, "Snow", false);
			GuiCheckBox(Rectangle{40, 155, 15, 15}, "Desert", false);
			GuiCheckBox(Rectangle{40, 175, 15, 15}, "Forest", false);
			GuiCheckBox(Rectangle{40, 195, 15, 15}, "Beach", false);
			GuiCheckBox(Rectangle{120, 82, 15, 15}, "Intro", false);
			GuiCheckBox(Rectangle{170, 82, 15, 15}, "Ending", false);
			GuiLabel(Rectangle{120, 105, 35, 15}, "Gems");
			GuiTextBox(Rectangle{120, 120, 35, 20},"0", 5, false);
			GuiLabel(Rectangle{170, 105, 50, 15}, "Lives");
			GuiTextBox(Rectangle{170, 120, 50, 20},"9999", 5, false);
			GuiButton(Rectangle{119, 144, 16, 20}, "0");
			GuiButton(Rectangle{138, 144, 16, 20}, "9");
			GuiButton(Rectangle{170, 144, 16, 20}, "5");
			GuiButton(Rectangle{188, 144, 31, 20}, "999");
			GuiButton(Rectangle{119, 174, 35, 20}, "max");
			GuiButton(Rectangle{169, 174, 50, 20}, "delete");
			GuiButton(Rectangle{119, 202, 35, 20}, "save");
			GuiButton(Rectangle{169, 202, 50, 20}, "reload");

			GuiGroupBox(Rectangle{240, 60, 220, 175}, "File2  [ 63-69420 ]");
			GuiGroupBox(Rectangle{250, 78, 90, 145}, "Level Progress");
			GuiCheckBox(Rectangle{270, 95, 15, 15}, "Castle", false);
			GuiCheckBox(Rectangle{270, 115, 15, 15}, "Moon", false);
			GuiCheckBox(Rectangle{270, 135, 15, 15}, "Snow", false);
			GuiCheckBox(Rectangle{270, 155, 15, 15}, "Desert", false);
			GuiCheckBox(Rectangle{270, 175, 15, 15}, "Forest", false);
			GuiCheckBox(Rectangle{270, 195, 15, 15}, "Beach", false);
			GuiCheckBox(Rectangle{350, 82, 15, 15}, "Intro", false);
			GuiCheckBox(Rectangle{400, 82, 15, 15}, "Ending", false);
			GuiLabel(Rectangle{350, 105, 35, 15}, "Gems");
			GuiTextBox(Rectangle{350, 120, 35, 20}, "0", 5, false);
			GuiLabel(Rectangle{400, 105, 50, 15}, "Lives");
			GuiTextBox(Rectangle{400, 120, 50, 20}, "0", 5, false);
			GuiButton(Rectangle{350, 144, 16, 20}, "0");
			GuiButton(Rectangle{369, 144, 16, 20}, "9");
			GuiButton(Rectangle{400, 144, 16, 20}, "5");
			GuiButton(Rectangle{419, 144, 31, 20}, "999");
			GuiButton(Rectangle{350, 174, 35, 20}, "max");
			GuiButton(Rectangle{400, 174, 50, 20}, "delete");
			GuiButton(Rectangle{350, 202, 35, 20}, "save");
			GuiButton(Rectangle{400, 202, 50, 20}, "reload");

			GuiGroupBox(Rectangle{470, 60, 220, 175}, "File3  [ 63-69420 ]");
			GuiGroupBox(Rectangle{480, 78, 90, 145}, "Level Progress");
			GuiCheckBox(Rectangle{500, 95, 15, 15}, "Castle", false);
			GuiCheckBox(Rectangle{500, 115, 15, 15}, "Moon", false);
			GuiCheckBox(Rectangle{500, 135, 15, 15}, "Snow", false);
			GuiCheckBox(Rectangle{500, 155, 15, 15}, "Desert", false);
			GuiCheckBox(Rectangle{500, 175, 15, 15}, "Forest", false);
			GuiCheckBox(Rectangle{500, 195, 15, 15}, "Beach", false);
			GuiCheckBox(Rectangle{580, 82, 15, 15}, "Intro", false);
			GuiCheckBox(Rectangle{630, 82, 15, 15}, "Ending", false);
			GuiLabel(Rectangle{580, 105, 40, 15}, "Gems");
			GuiTextBox(Rectangle{580, 120, 35, 20}, "0", 5, false);
			GuiLabel(Rectangle{630, 105, 45, 15}, "Lives");
			GuiTextBox(Rectangle{630, 120, 50, 20}, "0", 5, false);
			GuiButton(Rectangle{580, 143, 16, 20}, "0");
			GuiButton(Rectangle{599, 143, 16, 20}, "9");
			GuiButton(Rectangle{630, 144, 16, 20}, "5");
			GuiButton(Rectangle{649, 144, 31, 20}, "999");
			GuiButton(Rectangle{580, 173, 35, 20}, "max");
			GuiButton(Rectangle{630, 173, 50, 20}, "delete");
			GuiButton(Rectangle{580, 201, 35, 20}, "save");
			GuiButton(Rectangle{630, 201, 50, 20}, "reload");

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
