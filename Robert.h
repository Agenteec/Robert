// Robert.h : включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта.

#pragma once

#include <iostream>
#include <SFML/graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include <random>
#include <cmath>

// Масштаб: сколько пикселей в одном метре Box2D (в данном примере 100 пикселей = 1 метр)
const float SCALE = 100.0f;


// Управляющие последовательности ANSI для цветов
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// TODO: установите здесь ссылки на дополнительные заголовки, требующиеся для программы.
