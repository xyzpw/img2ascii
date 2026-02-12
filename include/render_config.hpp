#pragma once

#include "args.hpp"
#include "image.hpp"

void configureImage(Image&, const Args&);
ImageConfig makeImageConfig(const Args& args);
