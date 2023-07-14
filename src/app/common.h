#pragma once

/* COMMON INCLUDES */
/* C/C++ INCLUDES */
#include <stdint.h>
#include <string>
#include <iostream>
#include <memory>
#include <filesystem>
#include <thread>

/* PROJECT INCLUDES */
#include "log/log.h"

/* EXTERNAL INCLUDES */
#include "imgui/imgui.h"

/* COMMON DEFINES */
/* FIXME: What about moving defines to some 'constants' file? */
#define TMP_DIR			".tmp"
#define SIQ_EXTRACT_DIR TMP_DIR "/siq"
#define ASSETS_DIR		"assets"

/* OTHER STUFF */
namespace fs = std::filesystem;
