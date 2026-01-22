#include <ArduinoJson.h>
#include <yoga/Yoga.h>

// ---------- helpers: strings -> Yoga enums ----------

static YGFlexDirection parseFlexDirection(const char* s) {
  if (!s) return YGFlexDirectionColumn;
  if (strcmp(s, "row") == 0) return YGFlexDirectionRow;
  if (strcmp(s, "row-reverse") == 0) return YGFlexDirectionRowReverse;
  if (strcmp(s, "column-reverse") == 0) return YGFlexDirectionColumnReverse;
  return YGFlexDirectionColumn;
}

static YGJustify parseJustify(const char* s) {
  if (!s) return YGJustifyFlexStart;
  if (strcmp(s, "center") == 0) return YGJustifyCenter;
  if (strcmp(s, "flex-end") == 0) return YGJustifyFlexEnd;
  if (strcmp(s, "space-between") == 0) return YGJustifySpaceBetween;
  if (strcmp(s, "space-around") == 0) return YGJustifySpaceAround;
  if (strcmp(s, "space-evenly") == 0) return YGJustifySpaceEvenly;
  return YGJustifyFlexStart;
}

static YGAlign parseAlign(const char* s) {
  if (!s) return YGAlignStretch;
  if (strcmp(s, "flex-start") == 0) return YGAlignFlexStart;
  if (strcmp(s, "center") == 0) return YGAlignCenter;
  if (strcmp(s, "flex-end") == 0) return YGAlignFlexEnd;
  if (strcmp(s, "baseline") == 0) return YGAlignBaseline;
  if (strcmp(s, "stretch") == 0) return YGAlignStretch;
  return YGAlignStretch;
}

static YGWrap parseWrap(const char* s) {
  if (!s) return YGWrapNoWrap;
  if (strcmp(s, "wrap") == 0) return YGWrapWrap;
  if (strcmp(s, "wrap-reverse") == 0) return YGWrapWrapReverse;
  return YGWrapNoWrap;
}

static YGPositionType parsePositionType(const char* s) {
  if (!s) return YGPositionTypeRelative;
  if (strcmp(s, "absolute") == 0) return YGPositionTypeAbsolute;
  return YGPositionTypeRelative;
}

static YGDisplay parseDisplay(const char* s) {
  if (!s) return YGDisplayFlex;
  if (strcmp(s, "none") == 0) return YGDisplayNone;
  return YGDisplayFlex;
}

static YGOverflow parseOverflow(const char* s) {
  if (!s) return YGOverflowVisible;
  if (strcmp(s, "hidden") == 0) return YGOverflowHidden;
  if (strcmp(s, "scroll") == 0) return YGOverflowScroll;
  return YGOverflowVisible;
}

// ---------- helpers: "value objects" (px/%/auto) ----------

struct JsonSize {
  enum class Unit { Px, Percent, Auto, Undefined } unit = Unit::Undefined;
  float value = 0.0f;
};

static JsonSize parseSize(JsonVariant v) {
  JsonSize out;

  if (v.isNull()) return out;

  // allow numbers as shorthand: "width": 120  => px
  if (v.is<float>() || v.is<int>()) {
    out.unit = JsonSize::Unit::Px;
    out.value = v.as<float>();
    return out;
  }

  JsonObject o = v.as<JsonObject>();
  const char* unit = o["unit"] | nullptr;

  if (!unit) return out;

  if (strcmp(unit, "px") == 0) {
    out.unit = JsonSize::Unit::Px;
    out.value = o["value"] | 0.0f;
  } else if (strcmp(unit, "%") == 0) {
    out.unit = JsonSize::Unit::Percent;
    out.value = o["value"] | 0.0f;
  } else if (strcmp(unit, "auto") == 0) {
    out.unit = JsonSize::Unit::Auto;
  } else if (strcmp(unit, "undefined") == 0) {
    out.unit = JsonSize::Unit::Undefined;
  }

  return out;
}

static void applyWidth(YGNodeRef node, JsonVariant v) {
  JsonSize s = parseSize(v);
  switch (s.unit) {
    case JsonSize::Unit::Px:      YGNodeStyleSetWidth(node, s.value); break;
    case JsonSize::Unit::Percent: YGNodeStyleSetWidthPercent(node, s.value); break;
    case JsonSize::Unit::Auto:    YGNodeStyleSetWidthAuto(node); break;
    case JsonSize::Unit::Undefined: default: /* leave default */ break;
  }
}

static void applyHeight(YGNodeRef node, JsonVariant v) {
  JsonSize s = parseSize(v);
  switch (s.unit) {
    case JsonSize::Unit::Px:      YGNodeStyleSetHeight(node, s.value); break;
    case JsonSize::Unit::Percent: YGNodeStyleSetHeightPercent(node, s.value); break;
    case JsonSize::Unit::Auto:    YGNodeStyleSetHeightAuto(node); break;
    case JsonSize::Unit::Undefined: default: break;
  }
}

static void applyMinWidth(YGNodeRef node, JsonVariant v) {
  JsonSize s = parseSize(v);
  if (s.unit == JsonSize::Unit::Px) YGNodeStyleSetMinWidth(node, s.value);
  else if (s.unit == JsonSize::Unit::Percent) YGNodeStyleSetMinWidthPercent(node, s.value);
}

static void applyMinHeight(YGNodeRef node, JsonVariant v) {
  JsonSize s = parseSize(v);
  if (s.unit == JsonSize::Unit::Px) YGNodeStyleSetMinHeight(node, s.value);
  else if (s.unit == JsonSize::Unit::Percent) YGNodeStyleSetMinHeightPercent(node, s.value);
}

static void applyMaxWidth(YGNodeRef node, JsonVariant v) {
  JsonSize s = parseSize(v);
  if (s.unit == JsonSize::Unit::Px) YGNodeStyleSetMaxWidth(node, s.value);
  else if (s.unit == JsonSize::Unit::Percent) YGNodeStyleSetMaxWidthPercent(node, s.value);
}

static void applyMaxHeight(YGNodeRef node, JsonVariant v) {
  JsonSize s = parseSize(v);
  if (s.unit == JsonSize::Unit::Px) YGNodeStyleSetMaxHeight(node, s.value);
  else if (s.unit == JsonSize::Unit::Percent) YGNodeStyleSetMaxHeightPercent(node, s.value);
}

static void applyFlexBasis(YGNodeRef node, JsonVariant v) {
  JsonSize s = parseSize(v);
  switch (s.unit) {
    case JsonSize::Unit::Px:      YGNodeStyleSetFlexBasis(node, s.value); break;
    case JsonSize::Unit::Percent: YGNodeStyleSetFlexBasisPercent(node, s.value); break;
    case JsonSize::Unit::Auto:    YGNodeStyleSetFlexBasisAuto(node); break;
    default: break;
  }
}

// ---------- helpers: edges (margin/padding/border/position edges) ----------

static bool hasKey(JsonObject o, const char* key) {
  return o.containsKey(key) && !o[key].isNull();
}

static void applyEdgesMargin(YGNodeRef node, JsonObject edges) {
  if (hasKey(edges, "all"))        YGNodeStyleSetMargin(node, YGEdgeAll, edges["all"].as<float>());
  if (hasKey(edges, "horizontal")) YGNodeStyleSetMargin(node, YGEdgeHorizontal, edges["horizontal"].as<float>());
  if (hasKey(edges, "vertical"))   YGNodeStyleSetMargin(node, YGEdgeVertical, edges["vertical"].as<float>());
  if (hasKey(edges, "left"))       YGNodeStyleSetMargin(node, YGEdgeLeft, edges["left"].as<float>());
  if (hasKey(edges, "top"))        YGNodeStyleSetMargin(node, YGEdgeTop, edges["top"].as<float>());
  if (hasKey(edges, "right"))      YGNodeStyleSetMargin(node, YGEdgeRight, edges["right"].as<float>());
  if (hasKey(edges, "bottom"))     YGNodeStyleSetMargin(node, YGEdgeBottom, edges["bottom"].as<float>());
}

static void applyEdgesPadding(YGNodeRef node, JsonObject edges) {
  if (hasKey(edges, "all"))        YGNodeStyleSetPadding(node, YGEdgeAll, edges["all"].as<float>());
  if (hasKey(edges, "horizontal")) YGNodeStyleSetPadding(node, YGEdgeHorizontal, edges["horizontal"].as<float>());
  if (hasKey(edges, "vertical"))   YGNodeStyleSetPadding(node, YGEdgeVertical, edges["vertical"].as<float>());
  if (hasKey(edges, "left"))       YGNodeStyleSetPadding(node, YGEdgeLeft, edges["left"].as<float>());
  if (hasKey(edges, "top"))        YGNodeStyleSetPadding(node, YGEdgeTop, edges["top"].as<float>());
  if (hasKey(edges, "right"))      YGNodeStyleSetPadding(node, YGEdgeRight, edges["right"].as<float>());
  if (hasKey(edges, "bottom"))     YGNodeStyleSetPadding(node, YGEdgeBottom, edges["bottom"].as<float>());
}

static void applyEdgesBorder(YGNodeRef node, JsonObject edges) {
  if (hasKey(edges, "all"))        YGNodeStyleSetBorder(node, YGEdgeAll, edges["all"].as<float>());
  if (hasKey(edges, "horizontal")) YGNodeStyleSetBorder(node, YGEdgeHorizontal, edges["horizontal"].as<float>());
  if (hasKey(edges, "vertical"))   YGNodeStyleSetBorder(node, YGEdgeVertical, edges["vertical"].as<float>());
  if (hasKey(edges, "left"))       YGNodeStyleSetBorder(node, YGEdgeLeft, edges["left"].as<float>());
  if (hasKey(edges, "top"))        YGNodeStyleSetBorder(node, YGEdgeTop, edges["top"].as<float>());
  if (hasKey(edges, "right"))      YGNodeStyleSetBorder(node, YGEdgeRight, edges["right"].as<float>());
  if (hasKey(edges, "bottom"))     YGNodeStyleSetBorder(node, YGEdgeBottom, edges["bottom"].as<float>());
}

static void applyPositionEdges(YGNodeRef node, JsonObject pos) {
  // left/top/right/bottom accept px/%/auto in your schema, so use parseSize + SetPosition
  auto applyPosEdge = [&](const char* key, YGEdge edge) {
    if (!pos.containsKey(key)) return;
    JsonSize s = parseSize(pos[key]);
    if (s.unit == JsonSize::Unit::Px)      YGNodeStyleSetPosition(node, edge, s.value);
    else if (s.unit == JsonSize::Unit::Percent) YGNodeStyleSetPositionPercent(node, edge, s.value);
    // "auto"/undefined: do nothing (Yoga treats unspecified as auto-ish)
  };

  applyPosEdge("left", YGEdgeLeft);
  applyPosEdge("top", YGEdgeTop);
  applyPosEdge("right", YGEdgeRight);
  applyPosEdge("bottom", YGEdgeBottom);
}

// ---------- main: apply Yoga layout style ----------

void loadLayout(YGNodeRef node, JsonObject json);
