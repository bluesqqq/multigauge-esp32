#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <multigauge/graphics/GraphicsContext.h>

static inline uint32_t packRGBA(rgba c) {
    return (uint32_t(c.r) << 24) |
           (uint32_t(c.g) << 16) |
           (uint32_t(c.b) << 8)  |
           (uint32_t(c.a) << 0);
}

struct RecordedDrawCmd {
    enum Op : uint8_t {
        FillAll,
        DrawPixel,
        StrokeLine,
        FillWideLine,
        FillRect,
        StrokeRect,
        FillRoundRect1,
        FillRoundRect4,
        StrokeRoundRect1,
        StrokeRoundRect4,
        FillCircle,
        StrokeCircle,
        FillEllipse,
        StrokeEllipse,
        FillRing,
        StrokeRing,
        FillArc,
        StrokeArc,
        StrokeTriangle,
        FillTriangle,
        DrawText,
        SetClipRect,
        ClearClipRect,
        DrawImageXY,
        DrawImageXYWH
    } op;

    int i[8] = {0};        // coords, sizes
    float f[8] = {0.0f};   // thickness, radii, angles, pt
    uint32_t color = 0;    // packed RGBA

    std::string text;      // for DrawText
    std::string family;    // for DrawText

    int anchor = 0;        // Anchor (cast)
    int weight = 0;        // FontWeight (cast)
    int slant  = 0;        // FontSlant (cast)
};

class GraphicsContextRecording : public GraphicsContext {
public:
    std::vector<RecordedDrawCmd> cmds;

    GraphicsContextRecording(int w, int h) {
        width = w;
        height = h;
    }

    void clear() { cmds.clear(); }

    //----------[ FILL ]----------//
    void fillAll(rgba color) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::FillAll; c.color = packRGBA(color);
        cmds.push_back(std::move(c));
    }

    //----------[ PIXEL ]----------//
    void drawPixel(int x, int y, rgba color) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::DrawPixel;
        c.i[0]=x; c.i[1]=y; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    //----------[ LINE ]----------//
    void strokeLine(int x0, int y0, int x1, int y1, rgba color) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::StrokeLine;
        c.i[0]=x0; c.i[1]=y0; c.i[2]=x1; c.i[3]=y1; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    void fillWideLine(int x0, int y0, int x1, int y1, rgba color, float thickness) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::FillWideLine;
        c.i[0]=x0; c.i[1]=y0; c.i[2]=x1; c.i[3]=y1; c.f[0]=thickness; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    //----------[ RECTANGLE ]----------//
    void fillRect(int x, int y, int w, int h, rgba color) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::FillRect;
        c.i[0]=x; c.i[1]=y; c.i[2]=w; c.i[3]=h; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    void strokeRect(int x, int y, int w, int h, rgba color, float thickness) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::StrokeRect;
        c.i[0]=x; c.i[1]=y; c.i[2]=w; c.i[3]=h; c.f[0]=thickness; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    void fillRoundRect(int x, int y, int w, int h, float radius, rgba color) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::FillRoundRect1;
        c.i[0]=x; c.i[1]=y; c.i[2]=w; c.i[3]=h; c.f[0]=radius; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    void fillRoundRect(int x, int y, int w, int h, float r1, float r2, float r3, float r4, rgba color) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::FillRoundRect4;
        c.i[0]=x; c.i[1]=y; c.i[2]=w; c.i[3]=h;
        c.f[0]=r1; c.f[1]=r2; c.f[2]=r3; c.f[3]=r4; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    void strokeRoundRect(int x, int y, int w, int h, float radius, rgba color, float thickness) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::StrokeRoundRect1;
        c.i[0]=x; c.i[1]=y; c.i[2]=w; c.i[3]=h; c.f[0]=radius; c.f[1]=thickness; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    void strokeRoundRect(int x, int y, int w, int h, float r1, float r2, float r3, float r4, rgba color, float thickness) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::StrokeRoundRect4;
        c.i[0]=x; c.i[1]=y; c.i[2]=w; c.i[3]=h;
        c.f[0]=r1; c.f[1]=r2; c.f[2]=r3; c.f[3]=r4; c.f[4]=thickness; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    //----------[ CIRCLE ]----------//
    void fillCircle(int cx, int cy, int r, rgba color) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::FillCircle;
        c.i[0]=cx; c.i[1]=cy; c.i[2]=r; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    void strokeCircle(int cx, int cy, int r, rgba color, float thickness) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::StrokeCircle;
        c.i[0]=cx; c.i[1]=cy; c.i[2]=r; c.f[0]=thickness; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    //----------[ ELLIPSE ]----------//
    void fillEllipse(int cx, int cy, int rx, int ry, rgba color) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::FillEllipse;
        c.i[0]=cx; c.i[1]=cy; c.i[2]=rx; c.i[3]=ry; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    void strokeEllipse(int cx, int cy, int rx, int ry, rgba color, float thickness) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::StrokeEllipse;
        c.i[0]=cx; c.i[1]=cy; c.i[2]=rx; c.i[3]=ry; c.f[0]=thickness; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    //----------[ RING ]----------//
    void fillRing(int cx, int cy, int r, rgba color) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::FillRing;
        c.i[0]=cx; c.i[1]=cy; c.i[2]=r; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    void strokeRing(int cx, int cy, int r, rgba color, float thickness) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::StrokeRing;
        c.i[0]=cx; c.i[1]=cy; c.i[2]=r; c.f[0]=thickness; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    //----------[ ARC ]----------//
    void fillArc(int cx, int cy, int r1, int r2, float start, float end, rgba color) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::FillArc;
        c.i[0]=cx; c.i[1]=cy; c.i[2]=r1; c.i[3]=r2; c.f[0]=start; c.f[1]=end; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    void strokeArc(int cx, int cy, int r1, int r2, float start, float end, rgba color, float thickness) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::StrokeArc;
        c.i[0]=cx; c.i[1]=cy; c.i[2]=r1; c.i[3]=r2; c.f[0]=start; c.f[1]=end; c.f[2]=thickness; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    //----------[ TRIANGLE ]----------//
    void strokeTriangle(int x0, int y0, int x1, int y1, int x2, int y2, rgba color, float thickness) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::StrokeTriangle;
        c.i[0]=x0; c.i[1]=y0; c.i[2]=x1; c.i[3]=y1; c.i[4]=x2; c.i[5]=y2; c.f[0]=thickness; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, rgba color) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::FillTriangle;
        c.i[0]=x0; c.i[1]=y0; c.i[2]=x1; c.i[3]=y1; c.i[4]=x2; c.i[5]=y2; c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    //----------[ TEXT ]----------//
    float getTextWidth(const char* text, std::string family, float pt, FontWeight weight, FontSlant slant) override {
        (void)text; (void)family; (void)pt; (void)weight; (void)slant;
        return 0.0f;
    }

    void drawText(const char* text, int x, int y, std::string family, float pt,
                  FontWeight weight, FontSlant slant, rgba color,
                  Anchor anchor = Anchor::TopLeft) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::DrawText;
        c.i[0]=x; c.i[1]=y;
        c.text = text ? text : "";
        c.family = std::move(family);
        c.f[0]=pt;
        c.weight = (int)weight;
        c.slant  = (int)slant;
        c.anchor = (int)anchor;
        c.color=packRGBA(color);
        cmds.push_back(std::move(c));
    }

    //----------[ IMAGE ]----------//
    Image createNativeImage(const rgba* pixels, int w, int h) override {
        (void)pixels; (void)w; (void)h;
        return Image();
    }

    void drawImage(const Image& img, int x, int y, Anchor anchor = Anchor::TopLeft) override {
        (void)img;
        RecordedDrawCmd c; c.op = RecordedDrawCmd::DrawImageXY;
        c.i[0]=x; c.i[1]=y; c.anchor=(int)anchor;
        cmds.push_back(std::move(c));
    }

    void drawImage(const Image& img, int x, int y, int w, int h) override {
        (void)img;
        RecordedDrawCmd c; c.op = RecordedDrawCmd::DrawImageXYWH;
        c.i[0]=x; c.i[1]=y; c.i[2]=w; c.i[3]=h;
        cmds.push_back(std::move(c));
    }

    //----------[ CLIP ]----------//
    void setClipRect(int x, int y, int w, int h) override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::SetClipRect;
        c.i[0]=x; c.i[1]=y; c.i[2]=w; c.i[3]=h;
        cmds.push_back(std::move(c));
    }

    void clearClipRect() override {
        RecordedDrawCmd c; c.op = RecordedDrawCmd::ClearClipRect;
        cmds.push_back(std::move(c));
    }
};

static inline void json_escape_append_recording(std::string& out, const std::string& s) {
    for (char ch : s) {
        switch (ch) {
            case '\"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += ch; break;
        }
    }
}

static inline std::string recording_to_json(const std::vector<RecordedDrawCmd>& cmds) {
    std::string out;
    out += "[";

    for (size_t i = 0; i < cmds.size(); ++i) {
        if (i) out += ",";
        const auto& c = cmds[i];

        switch (c.op) {
            case RecordedDrawCmd::FillAll:
                out += "{\"op\":\"fillAll\",\"color\":" + std::to_string(c.color) + "}";
                break;

            case RecordedDrawCmd::FillRect:
                out += "{\"op\":\"fillRect\",\"x\":" + std::to_string(c.i[0]) +
                       ",\"y\":" + std::to_string(c.i[1]) +
                       ",\"w\":" + std::to_string(c.i[2]) +
                       ",\"h\":" + std::to_string(c.i[3]) +
                       ",\"color\":" + std::to_string(c.color) + "}";
                break;

            case RecordedDrawCmd::StrokeLine:
                out += "{\"op\":\"strokeLine\",\"x0\":" + std::to_string(c.i[0]) +
                       ",\"y0\":" + std::to_string(c.i[1]) +
                       ",\"x1\":" + std::to_string(c.i[2]) +
                       ",\"y1\":" + std::to_string(c.i[3]) +
                       ",\"color\":" + std::to_string(c.color) + "}";
                break;

            case RecordedDrawCmd::DrawText:
                out += "{\"op\":\"text\",\"x\":" + std::to_string(c.i[0]) +
                       ",\"y\":" + std::to_string(c.i[1]) +
                       ",\"pt\":" + std::to_string((double)c.f[0]) +
                       ",\"color\":" + std::to_string(c.color) +
                       ",\"anchor\":" + std::to_string(c.anchor) +
                       ",\"weight\":" + std::to_string(c.weight) +
                       ",\"slant\":" + std::to_string(c.slant) +
                       ",\"family\":\"";
                json_escape_append_recording(out, c.family);
                out += "\",\"text\":\"";
                json_escape_append_recording(out, c.text);
                out += "\"}";
                break;

            case RecordedDrawCmd::SetClipRect:
                out += "{\"op\":\"setClipRect\",\"x\":" + std::to_string(c.i[0]) +
                       ",\"y\":" + std::to_string(c.i[1]) +
                       ",\"w\":" + std::to_string(c.i[2]) +
                       ",\"h\":" + std::to_string(c.i[3]) + "}";
                break;

            case RecordedDrawCmd::ClearClipRect:
                out += "{\"op\":\"clearClipRect\"}";
                break;

            default:
                out += "{\"op\":\"unhandled\",\"code\":" + std::to_string((int)c.op) + "}";
                break;
        }
    }

    out += "]";
    return out;
}
