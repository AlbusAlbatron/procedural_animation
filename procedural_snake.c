#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>


typedef struct {
    int line_pos;
    Vector2 center;
    int radius;
} Circle;


Circle* circle_generator(int circle_count, Vector2 first_pos, int first_radius){

    Circle* circle_line = (Circle*)malloc(circle_count * sizeof(Circle));
    if (circle_line == NULL) {
	printf("circle_generator: malloc failed");
	return NULL;
    }
    
   circle_line[0].line_pos = 0;
   circle_line[0].center = first_pos;
   circle_line[0].radius = first_radius;

    for (int i = 1; i < circle_count; i++){
	circle_line[i].line_pos = i;
	circle_line[i].center = Vector2Subtract(circle_line[i - 1].center, (Vector2){circle_line[i - 1].radius, 0.0f});
	circle_line[i].radius = (circle_line[i - 1].radius) * 0.8; 
    }

    return circle_line;
}


int motion(Circle* circle_line, int circle_count, Vector2 head_pos){
    
    Vector2 prev_pos = head_pos;
    Vector2 temp_pos;
    
    for (int i = 1; i < circle_count; i++){

	//Check if distance between to centres are greater than radius
	if (Vector2Distance(circle_line[i - 1].center, circle_line[i].center) > circle_line[i - 1].radius){

	    //Move circle i towards circle [i-1]'s last postion
	    temp_pos = circle_line[i].center;
	    circle_line[i].center = Vector2Lerp(circle_line[i].center, prev_pos, 0.03f);
	    prev_pos = temp_pos;
	     
	}
	else{
	    prev_pos = circle_line[i].center;
	}
    }
    
    return 0;
}





int main(void){
   //Initialisation
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    int circle_count = 40;

    InitWindow(screenWidth, screenHeight, "Test Window");

    Vector2 starting_pos = { 300.0f, 300.0f };
    Vector2 mouse_pos;
    Vector2 pos_offset = { -100.0f, -100.0f };

    Circle* circle_list = circle_generator(circle_count ,starting_pos, 40);

    SetTargetFPS(600);

    //Main loop
    while (!WindowShouldClose())
    {
	if (IsKeyPressed(KEY_H))
	{
	    if (IsCursorHidden()){
		ShowCursor();
	    }
	    else{
		HideCursor();
	    }
	}

	//Move head circle and prevent it from moving too fast	
	mouse_pos = GetMousePosition();
	circle_list[0].center = Vector2Lerp(circle_list[0].center, mouse_pos, 0.03f); 

	motion(circle_list, circle_count, circle_list[0].center);

	BeginDrawing();

	    ClearBackground(RAYWHITE);
	    

	    for(int i = 0; i < circle_count;i++){
		DrawCircleLines(circle_list[i].center.x, circle_list[i].center.y, circle_list[i].radius, BLACK);
		DrawCircle(circle_list[i].center.x, circle_list[i].center.y, 1, RED);
	    }

	    EndDrawing();
    }
    
    CloseWindow();
    free(circle_list);

    return 0;
}
