#include "slay/renderer/sdl_renderer.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "slay/font_manager.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <clay.h>
#include <vector>

static int NUM_CIRCLE_SEGMENTS = 16;

slay::SDLRenderer::~SDLRenderer() {
    if (m_renderer)
        SDL_DestroyRenderer(m_renderer);
    TTF_Quit();
}

bool slay::SDLRenderer::init() {
    if (!TTF_Init()) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }
    m_renderer = SDL_CreateRenderer(m_window, "vulkan");
    if (!m_renderer) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }
    m_textEngine = TTF_CreateRendererTextEngine(m_renderer);
    if (!m_textEngine) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    return true;
}

void slay::SDLRenderer::beginFrame() {
  SDL_SetRenderDrawColor(m_renderer, 20, 20, 24, 255);
  SDL_RenderClear(m_renderer);
}

void slay::SDLRenderer::endFrame() { SDL_RenderPresent(m_renderer); }

void slay::SDLRenderer::render(RenderCommandArray& commands) {

  for (int i = 0; i < commands.length; i++) {
    Clay_RenderCommand *command = Clay_RenderCommandArray_Get(&commands, i);

    switch (command->commandType) {
    case CLAY_RENDER_COMMAND_TYPE_RECTANGLE:
      drawRectangle(*command);
      break;
    case CLAY_RENDER_COMMAND_TYPE_BORDER:
        drawBorder(*command);
        break;
    case CLAY_RENDER_COMMAND_TYPE_IMAGE:
        drawImage(*command);
        break;
    case CLAY_RENDER_COMMAND_TYPE_TEXT:
        drawText(*command);
        break;
    case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START:
        startScissors(*command);
        break;
    case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END:
        endScissors(*command);
        break;
    }
  }
}

void slay::SDLRenderer::drawArc(const Clay_RenderCommand &cmd,
                               const SDL_FPoint center, const float radius,
                               const float startAngle, const float endAngle,
                               const float thickness, const Clay_Color color) {
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

  const float radStart = startAngle * (SDL_PI_F / 180.0f);
  const float radEnd = endAngle * (SDL_PI_F / 180.0f);

  const int numCircleSegments =
      SDL_max(NUM_CIRCLE_SEGMENTS,
              (int)(radius * 1.5f)); // increase circle segments for larger
                                     // circles, 1.5 is arbitrary.

  const float angleStep = (radEnd - radStart) / (float)numCircleSegments;
  const float thicknessStep =
      0.4f; // arbitrary value to avoid overlapping lines. Changing
            // THICKNESS_STEP or numCircleSegments might cause artifacts.

  for (float t = thicknessStep; t < thickness - thicknessStep;
       t += thicknessStep) {
    SDL_FPoint points[numCircleSegments + 1];
    const float clampedRadius = SDL_max(radius - t, 1.0f);

    for (int i = 0; i <= numCircleSegments; i++) {
      const float angle = radStart + i * angleStep;
      points[i] =
          (SDL_FPoint){SDL_roundf(center.x + SDL_cosf(angle) * clampedRadius),
                       SDL_roundf(center.y + SDL_sinf(angle) * clampedRadius)};
    }
    SDL_RenderLines(m_renderer, points, numCircleSegments + 1);
  }
}

void slay::SDLRenderer::drawRoundedRect(const Clay_RenderCommand &cmd,
                                       const SDL_FRect &rect) {
  int indexCount = 0, vertexCount = 0;
  SDL_FColor color = {
      .r = cmd.renderData.rectangle.backgroundColor.r / 255.0f,
      .g = cmd.renderData.rectangle.backgroundColor.g / 255.0f,
      .b = cmd.renderData.rectangle.backgroundColor.b / 255.0f,
      .a = cmd.renderData.rectangle.backgroundColor.a / 255.0f,
  };

  const float minRadius = SDL_min(rect.w, rect.h) / 2.0f;
  const float clampedRadius =
      SDL_min(cmd.renderData.rectangle.cornerRadius.topLeft, minRadius);
  const int numCircleSegments =
      SDL_max(NUM_CIRCLE_SEGMENTS, (int)clampedRadius * 0.5f);

  int totalVertices = 4 + (4 * (numCircleSegments * 2)) + 2 * 4;
  int totalIndices = 6 + (4 * (numCircleSegments * 3)) + 6 * 4;

  std::vector<SDL_Vertex> vertices;
  vertices.resize(totalVertices);
  std::vector<int> indices;
  indices.resize(totalIndices);

  vertices[vertexCount++] = (SDL_Vertex){
      {rect.x + clampedRadius, rect.y + clampedRadius}, color, {0, 0}};
  vertices[vertexCount++] = (SDL_Vertex){
      {rect.x + rect.w - clampedRadius, rect.y + clampedRadius}, color, {1, 0}};
  vertices[vertexCount++] = (SDL_Vertex){
      {rect.x + rect.w - clampedRadius, rect.y + rect.h - clampedRadius},
      color,
      {1, 1}};
  vertices[vertexCount++] = (SDL_Vertex){
      {rect.x + clampedRadius, rect.y + rect.h - clampedRadius}, color, {0, 1}};

  indices[indexCount++] = 0;
  indices[indexCount++] = 1;
  indices[indexCount++] = 3;
  indices[indexCount++] = 1;
  indices[indexCount++] = 2;
  indices[indexCount++] = 3;

  const float step = (SDL_PI_F / 2) / numCircleSegments;
  for (int i = 0; i < numCircleSegments; i++) {
    const float angle1 = (float)i * step;
    const float angle2 = ((float)i + 1.0f) * step;

    for (int j = 0; j < 4; j++) {
      float cx, cy, signX, signY;

      switch (j) {
      case 0:
        cx = rect.x + clampedRadius;
        cy = rect.y + clampedRadius;
        signX = -1;
        signY = -1;
        break; // Top-left
      case 1:
        cx = rect.x + rect.w - clampedRadius;
        cy = rect.y + clampedRadius;
        signX = 1;
        signY = -1;
        break; // Top-right
      case 2:
        cx = rect.x + rect.w - clampedRadius;
        cy = rect.y + rect.h - clampedRadius;
        signX = 1;
        signY = 1;
        break; // Bottom-right
      case 3:
        cx = rect.x + clampedRadius;
        cy = rect.y + rect.h - clampedRadius;
        signX = -1;
        signY = 1;
        break; // Bottom-left
      default:
        return;
      }

      vertices[vertexCount++] =
          (SDL_Vertex){{cx + SDL_cosf(angle1) * clampedRadius * signX,
                        cy + SDL_sinf(angle1) * clampedRadius * signY},
                       color,
                       {0, 0}};
      vertices[vertexCount++] =
          (SDL_Vertex){{cx + SDL_cosf(angle2) * clampedRadius * signX,
                        cy + SDL_sinf(angle2) * clampedRadius * signY},
                       color,
                       {0, 0}};

      indices[indexCount++] =
          j; // Connect to corresponding central rectangle vertex
      indices[indexCount++] = vertexCount - 2;
      indices[indexCount++] = vertexCount - 1;
    }
  }

  // Define edge rectangles
  //  Top edge
  vertices[vertexCount++] =
      (SDL_Vertex){{rect.x + clampedRadius, rect.y}, color, {0, 0}}; // TL
  vertices[vertexCount++] = (SDL_Vertex){
      {rect.x + rect.w - clampedRadius, rect.y}, color, {1, 0}}; // TR

  indices[indexCount++] = 0;
  indices[indexCount++] = vertexCount - 2; // TL
  indices[indexCount++] = vertexCount - 1; // TR
  indices[indexCount++] = 1;
  indices[indexCount++] = 0;
  indices[indexCount++] = vertexCount - 1; // TR
  // Right edge
  vertices[vertexCount++] = (SDL_Vertex){
      {rect.x + rect.w, rect.y + clampedRadius}, color, {1, 0}}; // RT
  vertices[vertexCount++] = (SDL_Vertex){
      {rect.x + rect.w, rect.y + rect.h - clampedRadius}, color, {1, 1}}; // RB

  indices[indexCount++] = 1;
  indices[indexCount++] = vertexCount - 2; // RT
  indices[indexCount++] = vertexCount - 1; // RB
  indices[indexCount++] = 2;
  indices[indexCount++] = 1;
  indices[indexCount++] = vertexCount - 1; // RB
  // Bottom edge
  vertices[vertexCount++] = (SDL_Vertex){
      {rect.x + rect.w - clampedRadius, rect.y + rect.h}, color, {1, 1}}; // BR
  vertices[vertexCount++] = (SDL_Vertex){
      {rect.x + clampedRadius, rect.y + rect.h}, color, {0, 1}}; // BL

  indices[indexCount++] = 2;
  indices[indexCount++] = vertexCount - 2; // BR
  indices[indexCount++] = vertexCount - 1; // BL
  indices[indexCount++] = 3;
  indices[indexCount++] = 2;
  indices[indexCount++] = vertexCount - 1; // BL
  // Left edge
  vertices[vertexCount++] = (SDL_Vertex){
      {rect.x, rect.y + rect.h - clampedRadius}, color, {0, 1}}; // LB
  vertices[vertexCount++] =
      (SDL_Vertex){{rect.x, rect.y + clampedRadius}, color, {0, 0}}; // LT

  indices[indexCount++] = 3;
  indices[indexCount++] = vertexCount - 2; // LB
  indices[indexCount++] = vertexCount - 1; // LT
  indices[indexCount++] = 0;
  indices[indexCount++] = 3;
  indices[indexCount++] = vertexCount - 1;

  SDL_RenderGeometry(m_renderer, nullptr, vertices.data(), vertexCount,
                     indices.data(), indexCount);
}

void slay::SDLRenderer::drawRectangle(const Clay_RenderCommand &cmd) {
  const Clay_RectangleRenderData &rectangleData = cmd.renderData.rectangle;
  SDL_FRect drawRect = {
      .x = cmd.boundingBox.x,
      .y = cmd.boundingBox.y,
      .w = cmd.boundingBox.width,
      .h = cmd.boundingBox.height,
  };

  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(m_renderer, rectangleData.backgroundColor.r,
                         rectangleData.backgroundColor.g,
                         rectangleData.backgroundColor.b,
                         rectangleData.backgroundColor.a);
  if (rectangleData.cornerRadius.topLeft > 0) {
    drawRoundedRect(cmd, drawRect);
  } else {
    SDL_RenderFillRect(m_renderer, &drawRect);
  }
}

void slay::SDLRenderer::drawBorder(const Clay_RenderCommand &cmd) {
  const Clay_BorderRenderData &borderData = cmd.renderData.border;
  SDL_FRect drawRect = {
      .x = cmd.boundingBox.x,
      .y = cmd.boundingBox.y,
      .w = cmd.boundingBox.width,
      .h = cmd.boundingBox.height,
  };

  const float minRadius = SDL_min(drawRect.w, drawRect.h) / 2.0f;
  const Clay_CornerRadius clampedRadius = {
      .topLeft = SDL_min(borderData.cornerRadius.topLeft, minRadius),
      .topRight = SDL_min(borderData.cornerRadius.topRight, minRadius),
      .bottomLeft = SDL_min(borderData.cornerRadius.bottomLeft, minRadius),
      .bottomRight = SDL_min(borderData.cornerRadius.bottomRight, minRadius)};

  SDL_SetRenderDrawColor(m_renderer, borderData.color.r, borderData.color.g,
                         borderData.color.b, borderData.color.a);
  if (borderData.width.left > 0) {
    const float starting_y = drawRect.y + clampedRadius.topLeft;
    const float length =
        drawRect.h - clampedRadius.topLeft - clampedRadius.bottomLeft;
    SDL_FRect line = {drawRect.x - 1, starting_y,
                      static_cast<float>(borderData.width.left), length};
    SDL_RenderFillRect(m_renderer, &line);
  }
  if (borderData.width.right > 0) {
    const float starting_x = drawRect.x + drawRect.w -
                             static_cast<float>(borderData.width.right) + 1;
    const float starting_y = drawRect.y + clampedRadius.topRight;
    const float length =
        drawRect.h - clampedRadius.topRight - clampedRadius.bottomRight;
    SDL_FRect line = {starting_x, starting_y,
                      static_cast<float>(borderData.width.right), length};
    SDL_RenderFillRect(m_renderer, &line);
  }
  if (borderData.width.top > 0) {
    const float starting_x = drawRect.x + clampedRadius.topLeft;
    const float length =
        drawRect.w - clampedRadius.topLeft - clampedRadius.topRight;
    SDL_FRect line = {starting_x, drawRect.y - 1, length,
                      static_cast<float>(borderData.width.top)};
    SDL_RenderFillRect(m_renderer, &line);
  }
  if (borderData.width.bottom > 0) {
    const float starting_x = drawRect.x + clampedRadius.bottomLeft;
    const float starting_y = drawRect.y + drawRect.h -
                             static_cast<float>(borderData.width.bottom) + 1;
    const float length =
        drawRect.w - clampedRadius.bottomLeft - clampedRadius.bottomRight;
    SDL_FRect line = {starting_x, starting_y, length,
                      static_cast<float>(borderData.width.bottom)};
    SDL_RenderFillRect(m_renderer, &line);
  }

  if (borderData.cornerRadius.topLeft > 0) {
    const float centerX = drawRect.x + clampedRadius.topLeft - 1;
    const float centerY = drawRect.y + clampedRadius.topLeft - 1;
    drawArc(cmd, (SDL_FPoint){centerX, centerY},
                       clampedRadius.topLeft, 180.0f, 270.0f,
                       borderData.width.top, borderData.color);
  }
  if (borderData.cornerRadius.topRight > 0) {
    const float centerX = drawRect.x + drawRect.w - clampedRadius.topRight;
    const float centerY = drawRect.y + clampedRadius.topRight - 1;
    drawArc(cmd, (SDL_FPoint){centerX, centerY},
                       clampedRadius.topRight, 270.0f, 360.0f, borderData.width.top,
                       borderData.color);
  }
  if (borderData.cornerRadius.bottomLeft > 0) {
    const float centerX = drawRect.x + clampedRadius.bottomLeft - 1;
    const float centerY = drawRect.y + drawRect.h - clampedRadius.bottomLeft;
    drawArc(cmd, (SDL_FPoint){centerX, centerY},
                       clampedRadius.bottomLeft, 90.0f, 180.0f,
                       borderData.width.bottom, borderData.color);
  }
  if (borderData.cornerRadius.bottomRight > 0) {
    const float centerX = drawRect.x + drawRect.w - clampedRadius.bottomRight;
    const float centerY = drawRect.y + drawRect.h - clampedRadius.bottomRight;
    drawArc(cmd, (SDL_FPoint){centerX, centerY},
                       clampedRadius.bottomRight, 0.0f, 90.0f,
                       borderData.width.bottom, borderData.color);
  }
}

void slay::SDLRenderer::drawImage(const Clay_RenderCommand &cmd) {
    SDL_Texture* image = static_cast<SDL_Texture*>(cmd.renderData.image.imageData);
    SDL_FRect drawRect = {
        .x = cmd.boundingBox.x,
        .y = cmd.boundingBox.y,
        .w = cmd.boundingBox.width,
        .h = cmd.boundingBox.height,
    };

    SDL_RenderTexture(m_renderer, image, nullptr, &drawRect);
}

void slay::SDLRenderer::startScissors(const Clay_RenderCommand& cmd) {
    SDL_Rect drawRect = {
        .x = static_cast<int>(cmd.boundingBox.x),
        .y = static_cast<int>(cmd.boundingBox.y),
        .w = static_cast<int>(cmd.boundingBox.width),
        .h = static_cast<int>(cmd.boundingBox.height),
    };

    SDL_SetRenderClipRect(m_renderer, &drawRect);
}

void slay::SDLRenderer::endScissors(const Clay_RenderCommand& cmd) {
    SDL_SetRenderClipRect(m_renderer, nullptr);
}

void slay::SDLRenderer::drawText(const Clay_RenderCommand& cmd) {
    const Clay_TextRenderData &textData = cmd.renderData.text;
    SDL_FRect drawRect = {
        .x = cmd.boundingBox.x,
        .y = cmd.boundingBox.y,
        .w = cmd.boundingBox.width,
        .h = cmd.boundingBox.height,
    };

    TTF_Font* font = m_fontManager.resolve(slay::FontId{textData.fontId}, textData.fontSize);
    if (!font) {
        return;
    }
    TTF_Text* text = TTF_CreateText(m_textEngine, font, textData.stringContents.chars, textData.stringContents.length);
    TTF_SetTextColorFloat(text, textData.textColor.r, textData.textColor.g, textData.textColor.b, textData.textColor.a);
    TTF_DrawRendererText(text, drawRect.x, drawRect.y);
    TTF_DestroyText(text);
}
