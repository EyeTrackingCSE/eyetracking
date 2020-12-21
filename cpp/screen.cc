#include "screen.h"

Screen::Screen(float w, float h)
{
    Screen::height = h;
    Screen::width = w;
    Screen::offset = 0.0f;

    // Init the tobii interaction library
    Screen::tobii = IL::UniqueInteractionLibPtr(IL::CreateInteractionLib(IL::FieldOfUse::Interactive));

    Screen::tobii->CoordinateTransformAddOrUpdateDisplayArea(Screen::width, Screen::height);
    Screen::tobii->CoordinateTransformSetOriginOffset(Screen::offset, Screen::offset);
}

Screen::~Screen() {}

/**
 * Binds the Screen object to v8, so it can be created
 * from a node process.
 * */
void Screen::Init(v8::Local<v8::Object> exports)
{
    v8::Isolate *isolate = exports->GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    // Enable Screen object to be created at runtime by v8.
    v8::Local<v8::ObjectTemplate> addon_data_tpl = v8::ObjectTemplate::New(isolate);
    addon_data_tpl->SetInternalFieldCount(5);
    v8::Local<v8::Object> addon_data = addon_data_tpl->NewInstance(context).ToLocalChecked();

    // Template function for Screen::New
    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, Screen::New, addon_data);
    tpl->SetClassName(v8::String::NewFromUtf8(isolate, "Screen").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(5);

    // Set Screen prototype functions
    NODE_SET_PROTOTYPE_METHOD(tpl, "GetHeight", Screen::GetHeight);
    NODE_SET_PROTOTYPE_METHOD(tpl, "SetHeight", Screen::SetHeight);
    NODE_SET_PROTOTYPE_METHOD(tpl, "GetWidth", Screen::GetWidth);
    NODE_SET_PROTOTYPE_METHOD(tpl, "SetWidth", Screen::SetWidth);

    v8::Local<v8::Function> construct = tpl->GetFunction(context).ToLocalChecked();
    addon_data->SetInternalField(0, construct);

    v8::Local<v8::String> object_name = v8::String::NewFromUtf8(isolate, "Screen").ToLocalChecked();
    exports->Set(context, object_name, construct).FromJust();
}

/**
 * JS constructor for creating a new Screen object as
 * let screen = new Screen(1920.0, 1080.0);
 * */
void Screen::New(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    // Screen must be called with the new keyboard
    // i.e. let myScreen = new Screen(1920, 1080)
    if (!args.IsConstructCall())
    {
        std::cout << "Please init Screen with the 'new' keyword" << std::endl;
        return;
    }

    // Get the width
    float w = 0.0f;
    if (!args[0]->IsUndefined())
        w = args[0]->NumberValue(context).FromMaybe(0.0f);

    // Get the height;
    float h = 0.0f;
    if (!args[1]->IsUndefined())
        h = args[1]->NumberValue(context).FromMaybe(0.0f);

    // Return the Screen instance
    Screen *s = new Screen(w, h);
    s->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
}

/**
 * Return the Screen::height;
 */
void Screen::GetHeight(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();

    Screen *s = ObjectWrap::Unwrap<Screen>(args.Holder());

    v8::Local<v8::Number> height = v8::Number::New(isolate, s->height);

    args.GetReturnValue().Set(height);
}

/**
 * Set Screen::Height
 */
void Screen::SetHeight(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    Screen *s = ObjectWrap::Unwrap<Screen>(args.Holder());

    float h = 0.0f;
    if (!args[0]->IsUndefined())
        h = args[0]->NumberValue(context).FromMaybe(0.0f);

    s->height = h;
}

/**
 * Set the Screen::width
 * */
void Screen::GetWidth(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();

    Screen *s = ObjectWrap::Unwrap<Screen>(args.Holder());

    v8::Local<v8::Number> width = v8::Number::New(isolate, s->width);

    args.GetReturnValue().Set(width);
}

/**
 * Set the Screen::Width
 * */
void Screen::SetWidth(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate * isolate = args.GetIsolate();
    v8::Local<v8::Context> ctx = isolate->GetCurrentContext();

    Screen * s = ObjectWrap::Unwrap<Screen>(args.Holder());

    float w = 0.0f;
    if (!args[0]->IsUndefined())
        w = args[0]->NumberValue(ctx).FromMaybe(0.0f);

    s->width = w;
}

void Screen::AddRectangle(const v8::FunctionCallbackInfo<v8::Value>& args) {

}

void Screen::Listen(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate * isolate = args.GetIsolate();
}

// void Screen::AddRectangle(float x, float y, float w, float h, int id)
// {
//     IL::InteractorId rect_id = id;

//     IL::Rectangle rect = {x, y, w, h};

//     Screen::tobii->BeginInteractorUpdates();

//     Screen::tobii->AddOrUpdateInteractor(rect_id, rect, 0.0f);

//     Screen::tobii->CommitInteractorUpdates();
// }
