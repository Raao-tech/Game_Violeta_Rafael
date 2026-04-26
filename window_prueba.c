#include "./headers/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "headers/raygui.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argn, char* argv[]){
    if(argn < 3) return 1;

    int ancho = atoi(argv[1]);
    int alto = atoi(argv[2]);
    
    InitWindow(ancho, alto, "Proyecto Raylib");
    SetTargetFPS(60); // Importante para que el movimiento sea fluido

    int x = 0, y = 0;
    int ancho_cuadrado = 20, alto_cuadrado = 20;
    int speed = 1;
    int currentMenu = 0;

    while (!WindowShouldClose())
    {

        // --- Lógica de Actualización ---
        if (currentMenu == 1) {
            if (IsKeyDown(KEY_RIGHT) && (x + ((speed) *ancho_cuadrado) < ancho)) x += ((speed * ancho_cuadrado));
            if (IsKeyDown(KEY_LEFT) && (x - (ancho_cuadrado*speed) > 0)) x -= (ancho_cuadrado*speed);
            if (IsKeyDown(KEY_UP) && (y -  ((speed) *alto_cuadrado)> 0)) y -= (alto_cuadrado * speed);
            if (IsKeyDown(KEY_DOWN) && (y + ((speed) *alto_cuadrado) < alto)) y +=  ((speed) *alto_cuadrado);
        }

        // --- Renderizado ---
        BeginDrawing();
            ClearBackground(RAYWHITE);

            for (int i = 0; i < ancho; i++)
            {
                DrawLine(i*ancho_cuadrado, 0, i*ancho_cuadrado, alto, BLACK); /*lines  of the width*/
                if (i < alto) DrawLine(0, i*alto_cuadrado, ancho, i*ancho_cuadrado, BLACK); /*lines  of the height*/

            }
            

            if (currentMenu == 0) {
                DrawText("MENU PRINCIPAL", ancho/2 - 70, 100, 20, DARKGRAY);
                if (GuiButton((Rectangle){ ancho/2 - 100, 150, 200, 50 }, "Jugar")) currentMenu = 1;
                if (GuiButton((Rectangle){ ancho/2 - 100, 220, 200, 50 }, "Salir")) break;
            } 
            else if (currentMenu == 1) {
                DrawRectangle(x, y, ancho_cuadrado, alto_cuadrado, RED);
                DrawText("Usa las flechas para moverte", 10, 10, 20, LIGHTGRAY);
                
                if (GuiButton((Rectangle){ ancho - 110, 10, 100, 30 }, "Volver")) currentMenu = 0;
            }
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
