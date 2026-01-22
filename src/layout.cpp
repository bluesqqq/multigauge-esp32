#include "layout.h"

void loadLayout(YGNodeRef node, JsonObject json) {
  if (!node) return;

  JsonObject style = json["style"].as<JsonObject>();
  if (style.isNull()) return;

  // size
  if (style.containsKey("width"))     applyWidth(node, style["width"]);
  if (style.containsKey("height"))    applyHeight(node, style["height"]);
  if (style.containsKey("minWidth"))  applyMinWidth(node, style["minWidth"]);
  if (style.containsKey("minHeight")) applyMinHeight(node, style["minHeight"]);
  if (style.containsKey("maxWidth"))  applyMaxWidth(node, style["maxWidth"]);
  if (style.containsKey("maxHeight")) applyMaxHeight(node, style["maxHeight"]);
  if (style.containsKey("aspectRatio")) YGNodeStyleSetAspectRatio(node, style["aspectRatio"].as<float>());

  // display / overflow / direction (optional but useful)
  if (style.containsKey("display"))  YGNodeStyleSetDisplay(node, parseDisplay(style["display"] | nullptr));
  if (style.containsKey("overflow")) YGNodeStyleSetOverflow(node, parseOverflow(style["overflow"] | nullptr));
  if (style.containsKey("direction")) {
    const char* d = style["direction"] | nullptr;
    if (d && strcmp(d, "rtl") == 0) YGNodeStyleSetDirection(node, YGDirectionRTL);
    else if (d && strcmp(d, "ltr") == 0) YGNodeStyleSetDirection(node, YGDirectionLTR);
    else YGNodeStyleSetDirection(node, YGDirectionInherit);
  }

  // flex block
  if (style.containsKey("flex")) {
    JsonObject flex = style["flex"].as<JsonObject>();
    if (!flex.isNull()) {
      YGNodeStyleSetFlexDirection(node, parseFlexDirection(flex["direction"] | nullptr));
      YGNodeStyleSetJustifyContent(node, parseJustify(flex["justify"] | nullptr));
      YGNodeStyleSetAlignItems(node, parseAlign(flex["align"] | nullptr));
      if (flex.containsKey("alignSelf"))   YGNodeStyleSetAlignSelf(node, parseAlign(flex["alignSelf"] | nullptr));
      if (flex.containsKey("alignContent")) YGNodeStyleSetAlignContent(node, parseAlign(flex["alignContent"] | nullptr));
      YGNodeStyleSetFlexWrap(node, parseWrap(flex["wrap"] | nullptr));

      if (flex.containsKey("grow"))   YGNodeStyleSetFlexGrow(node, flex["grow"].as<float>());
      if (flex.containsKey("shrink")) YGNodeStyleSetFlexShrink(node, flex["shrink"].as<float>());
      if (flex.containsKey("basis"))  applyFlexBasis(node, flex["basis"]);
    }
  }

  // spacing
  if (style.containsKey("margin"))  applyEdgesMargin(node, style["margin"].as<JsonObject>());
  if (style.containsKey("padding")) applyEdgesPadding(node, style["padding"].as<JsonObject>());
  if (style.containsKey("border"))  applyEdgesBorder(node, style["border"].as<JsonObject>());

  // gap
  if (style.containsKey("gap")) {
    JsonObject gap = style["gap"].as<JsonObject>();
    if (!gap.isNull()) {
      if (gap.containsKey("row"))    YGNodeStyleSetGap(node, YGGutterRow, gap["row"].as<float>());
      if (gap.containsKey("column")) YGNodeStyleSetGap(node, YGGutterColumn, gap["column"].as<float>());
    }
  }

  // position
  if (style.containsKey("position")) {
    JsonObject pos = style["position"].as<JsonObject>();
    if (!pos.isNull()) {
      YGNodeStyleSetPositionType(node, parsePositionType(pos["type"] | nullptr));
      applyPositionEdges(node, pos);
    }
  }
}