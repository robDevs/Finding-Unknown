#include "raylib.h"
#include "view/View.h"

int main(void)
{
    View view;

    view.initView(1280, 720);

    while (!view.getWindowStatus())
    {
        // Update
            //coming soon

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            control_info gamepad = view.getControlInfo();

            if(gamepad.up_held) view.drawText("up", 190, 200, 20, BLACK);
            if(gamepad.left_held) view.drawText("left", 190, 230, 20, BLACK);
            if(gamepad.down_held) view.drawText("down", 190, 260, 20, BLACK);
            if(gamepad.right_held) view.drawText("right", 190, 290, 20, BLACK);
            if(gamepad.enter_held) view.drawText("enter", 190, 320, 20, BLACK);
            if(gamepad.back_held) view.drawText("back", 190, 350, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
