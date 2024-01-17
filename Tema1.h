#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        float translateX = 0, translateProjectile = 0;
        int squareSide = 120;

        glm::vec3 squareColor = glm::vec3(0.11f, 0.56f, 0.33f);
        glm::vec3 orange = glm::vec3(0.9f, 0.5f, 0);
        glm::vec3 blue = glm::vec3(0, 0.2f, 0.7f);
        glm::vec3 red = glm::vec3(0.7f, 0.1f, 0.1f);
        glm::vec3 purple = glm::vec3(0.6f, 0, 1.0f);
        glm::vec3 turqoise = glm::vec3(0.1f, 0.8f, 0.6f);

        int lifesRemaining = 3;

        bool addFirstStars = false;
        int starCount = 5; // stele pentru a cumpara romburi
        
        bool addedInitialStars = false; // added first 5 stars
        // daca sunt pe ecran

        int hexOrange[3] = { 0, 0, 0 };
        int hexBlue[3] = { 0, 0, 0 };
        int hexRed[3] = { 0, 0, 0 };
        int hexPurple[3] = { 0, 0, 0 };

        bool mousePressed1 = false, mousePressed2 = false, mousePressed3 = false, mousePressed4 = false;

        bool toDrag11 = false, toDrag12 = false, toDrag13 = false;
        bool toDrag21 = false, toDrag22 = false, toDrag23 = false;
        bool toDrag31 = false, toDrag32 = false, toDrag33 = false;

        int color11 = 0, color12 = 0, color13 = 0;
        int color21 = 0, color22 = 0, color23 = 0;
        int color31 = 0, color32 = 0, color33 = 0;

        int tipRombDrag;

        
        // coordonate romburi in patrate
        float dragRhombusX1, dragRhombusY1;
        float dragRhombusX2, dragRhombusY2;
        float dragRhombusX3, dragRhombusY3;
        float dragRhombusX4, dragRhombusY4;
        float dragRhombusX5, dragRhombusY5;
        float dragRhombusX6, dragRhombusY6;
        float dragRhombusX7, dragRhombusY7;
        float dragRhombusX8, dragRhombusY8;
        float dragRhombusX9, dragRhombusY9;

        //format: romb culoare linie coloana (daca am dat drop la un romb acolo)
        bool rombPurple11 = false, rombPurple12 = false, rombPurple13 = false;
        bool rombPurple21 = false, rombPurple22 = false, rombPurple23 = false;
        bool rombPurple31 = false, rombPurple32 = false, rombPurple33 = false;

        bool rombOrange11 = false, rombOrange12 = false, rombOrange13 = false;
        bool rombOrange21 = false, rombOrange22 = false, rombOrange23 = false;
        bool rombOrange31 = false, rombOrange32 = false, rombOrange33 = false;

        bool rombBlue11 = false, rombBlue12 = false, rombBlue13 = false;
        bool rombBlue21 = false, rombBlue22 = false, rombBlue23 = false;
        bool rombBlue31 = false, rombBlue32 = false, rombBlue33 = false;

        bool rombRed11 = false, rombRed12 = false, rombRed13 = false;
        bool rombRed21 = false, rombRed22 = false, rombRed23 = false;
        bool rombRed31 = false, rombRed32 = false, rombRed33 = false;

        int squareToRemove = 0;

    };
}   // namespace m1
