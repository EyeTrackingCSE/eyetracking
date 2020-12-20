#include "screen.h"

Screen::Screen(float h, float w)
{
    Screen::height = h;
    Screen::width = w;
    Screen::offset = 0.0f;

    // Init the tobii interaction library
    Screen::tobii = IL::UniqueInteractionLibPtr(IL::CreateInteractionLib(IL::FieldOfUse::Interactive));

    Screen::tobii->CoordinateTransformAddOrUpdateDisplayArea(Screen::width, Screen::height);
    Screen::tobii->CoordinateTransformSetOriginOffset(Screen::offset, Screen::offset);
}

float Screen::GetHeight()
{
    return Screen::height;
}

void Screen::SetHeight(float h)
{
    Screen::height = h;
}

float Screen::GetWidth()
{
    return Screen::width;
}

void Screen::SetWidth(float w)
{
    Screen::width = w;
}

void Screen::AddRectangle(float x, float y, float w, float h, int id) {
    IL::InteractorId rect_id = id;

    IL::Rectangle rect = {x, y, w, h};

    Screen::tobii->BeginInteractorUpdates();

    Screen::tobii->AddOrUpdateInteractor(rect_id, rect, 0.0f);

    Screen::tobii->CommitInteractorUpdates();
}