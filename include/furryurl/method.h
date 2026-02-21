#pragma once

namespace furryurl {

enum HTTPMethod : int {
    Get,
    Head,
    Post,
    Put,
    Delete,
    Options,
    Patch
};
using Method = HTTPMethod;

}