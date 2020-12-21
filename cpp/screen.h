/**
 * Anthony Vardaro 12/19/2020
 */

#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include <iostream>
#include <node.h>
#include <v8.h>
#include <node_object_wrap.h>
#include <interaction_lib/InteractionLib.h>
#include <interaction_lib/misc/InteractionLibPtr.h>

class Screen : public node::ObjectWrap
{
private:
    float height;
    float width;
    float offset;
    std::vector<IL::Rectangle> rectangles;
    IL::UniqueInteractionLibPtr tobii;

    Screen(float h, float w);
    ~Screen();

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void GetHeight(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void SetHeight(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void SetWidth(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void GetWidth(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void AddRectangle(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void Listen(const v8::FunctionCallbackInfo<v8::Value>& args);

public:

    static void Init(v8::Local<v8::Object> exports);

    // float GetHeight();
    // void SetHeight(float h);

    // float GetWidth();
    // void SetWidth(float w);

    // void AddRectangle(float x, float y, float w, float h, int id);

    // void Listen(const v8::FunctionCallbackInfo<v8::Value> &args);
};

#endif // SCREEN_H