#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdlib.h>


typedef struct {
    int line_pos;
    Vector2 center;
    int radius;
    Vector2 perp_bisector[2];
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
   circle_line[0].perp_bisector[0] = (Vector2){0.0f, 0.0f};
   circle_line[0].perp_bisector[1] = (Vector2){0.0f, 0.0f};


    for (int i = 1; i < circle_count; i++){
	circle_line[i].line_pos = i;
	circle_line[i].center = Vector2Subtract(circle_line[i - 1].center, (Vector2){circle_line[i - 1].radius, 0.0f});
	circle_line[i].radius = (circle_line[i - 1].radius) * 0.8;
	circle_line[i].perp_bisector[0] = (Vector2){0, 0};
	circle_line[i].perp_bisector[1] = (Vector2){0, 0};

    }

    return circle_line;
}


int motion(Circle* circle_line, int circle_count, Vector2 head_pos){
    
    Vector2 prev_pos = head_pos;
    Vector2 temp_pos;
    
    int total_steps = 360;
    float stepSizeDegrees = 360.0f / total_steps;

    for (int i = 1; i < circle_count; i++){

	//Check if distance between to centres are greater than radius
	if (Vector2Distance(circle_line[i - 1].center, circle_line[i].center) > circle_line[i - 1].radius){

	    //Move circle i towards circle [i-1]'s last postion
	    temp_pos = circle_line[i].center;
	    Vector2 direction = {prev_pos.x - temp_pos.x, prev_pos.y - temp_pos.y};
	    Vector2 norm_direction = Vector2Normalize(direction);

	    circle_line[i].center = Vector2Lerp(circle_line[i].center, prev_pos, 0.03f);
	    //Tangent position is centre +- radius (depending on rotation) 
	    //To get rotation of circle find where a diameter length line would be perpendicularly bisected
	    Vector2 norm_bisector_direction = { -norm_direction.y, norm_direction.x};		
	    circle_line[i].perp_bisector[0] = (Vector2){ temp_pos.x + (norm_bisector_direction.x * circle_line[i].radius),
				     temp_pos.y + (norm_bisector_direction.y * circle_line[i].radius)
	    };

	    norm_bisector_direction = (Vector2){ norm_direction.y, -norm_direction.x };
	    circle_line[i].perp_bisector[1] = (Vector2){ temp_pos.x + (norm_bisector_direction.x * circle_line[i].radius),
				     temp_pos.y + (norm_bisector_direction.y * circle_line[i].radius)
	    };


	    prev_pos = temp_pos;
	    } 
	
	else{
	    prev_pos = circle_line[i].center;
	}
    }
    
    return 0;
}

int draw_skin(Circle* circle_line, int circle_count){
    
    for(int i = 2; i < circle_count; i++){
	DrawLineV(circle_line[i].perp_bisector[0], circle_line[i - 1].perp_bisector[0], GREEN);
	DrawLineV(circle_line[i].perp_bisector[1], circle_line[i - 1].perp_bisector[1], GREEN);
    }


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
		DrawLineV(circle_list[i].perp_bisector[0], circle_list[i].perp_bisector[1], RED);
	    }
	    
	    draw_skin(circle_list, circle_count);

	    EndDrawing();
    }
    
    CloseWindow();
    free(circle_list);

    return 0;
}
