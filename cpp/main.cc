// hello.cc
#include <node.h>
#include <iostream>
#include <interaction_lib/InteractionLib.h>
#include <interaction_lib/misc/InteractionLibPtr.h>

#include "screen.h"

using namespace v8;

void Initialize(Local<Object> exports)
{
    Screen::Init(exports);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
