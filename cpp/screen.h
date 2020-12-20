/**
 * Anthony Vardaro 12/19/2020
 */

#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include <interaction_lib/InteractionLib.h>
#include <interaction_lib/misc/InteractionLibPtr.h>

class Screen
{
private:
    float height;
    float width;
    float offset;
    std::vector<IL::Rectangle> rectangles;
    IL::UniqueInteractionLibPtr tobii;


public:
    Screen(float h, float w);
    
    float GetHeight();
    void SetHeight(float h);

    float GetWidth();
    void SetWidth(float w);

    void AddRectangle(float x, float y, float w, float h, int id);

    void Listen();
};

#endif // SCREEN_H