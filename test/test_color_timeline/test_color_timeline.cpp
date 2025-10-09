#include <Arduino.h>
#include <unity.h>
#include "graphics/colors/ColorTimeline.h"
#include "graphics/colors/StaticColor.h"

void test_color_timeline_basic() {
    ColorTimeline timeline;
    TEST_ASSERT_EQUAL(0, timeline.size());

    // Add keyframes
    timeline.addKeyframe(0xFFFF, 0.0f);  // White
    timeline.addKeyframe(0x0000, 1.0f);  // Black

    TEST_ASSERT_EQUAL(2, timeline.size());

    TEST_ASSERT_EQUAL_UINT16(0xFFFF, timeline.getColor(0.0f));
    TEST_ASSERT_EQUAL_UINT16(0x0000, timeline.getColor(1.0f));

    // Midpoint should be blended (gray)
    uint16_t mid = timeline.getColor(0.5f);
    TEST_ASSERT_EQUAL_UINT16(0x7BEF, mid); // Roughly gray (tolerance)

    // Check start/end positions
    TEST_ASSERT_EQUAL_FLOAT(0.0f, timeline.getStartPosition());
    TEST_ASSERT_EQUAL_FLOAT(1.0f, timeline.getEndPosition());
}

void test_color_timeline_ordering() {
    ColorTimeline timeline;
    timeline.addKeyframe(0xF800, 1.0f);  // Red at 1.0
    timeline.addKeyframe(0x07E0, 0.5f);  // Green at 0.5
    timeline.addKeyframe(0x001F, 0.0f);  // Blue at 0.0

    const auto& keyframes = timeline.getTimeline();

    // Ensure ordering is by position (0.0, 0.5, 1.0)
    TEST_ASSERT_EQUAL_FLOAT(0.0f, keyframes[0].position);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, keyframes[1].position);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, keyframes[2].position);
}

void test_color_timeline_copy_and_assign() {
    ColorTimeline timelineA;
    timelineA.addKeyframe(0xF800, 0.0f);
    timelineA.addKeyframe(0x07E0, 1.0f);

    // Copy constructor
    ColorTimeline timelineB(timelineA);
    TEST_ASSERT_EQUAL(2, timelineB.size());
    TEST_ASSERT_EQUAL_UINT16(timelineA.getColor(0.5f), timelineB.getColor(0.5f));

    // Assignment operator
    ColorTimeline timelineC;
    timelineC = timelineA;
    TEST_ASSERT_EQUAL(2, timelineC.size());
    TEST_ASSERT_EQUAL_UINT16(timelineA.getColor(0.5f), timelineC.getColor(0.5f));
}

void test_color_timeline_blend_static() {
    ColorTimeline timelineA;
    timelineA.addKeyframe(0xF800, 0.0f);  // Red
    timelineA.addKeyframe(0x001F, 1.0f);  // Blue

    // Blend with white (0xFFFF) at 50%
    ColorTimeline blended = timelineA.blended(0xFFFF, 0.5f);

    TEST_ASSERT_EQUAL(timelineA.size(), blended.size());

    uint16_t colorA = timelineA.getColor(0.5f);
    uint16_t colorB = blended.getColor(0.5f);
    TEST_ASSERT_NOT_EQUAL(colorA, colorB);
}

void test_color_timeline_blend_two_timelines() {
    ColorTimeline t1;
    t1.addKeyframe(0xF800, 0.0f); // Red
    t1.addKeyframe(0x07E0, 1.0f); // Green

    ColorTimeline t2;
    t2.addKeyframe(0x001F, 0.0f); // Blue
    t2.addKeyframe(0xFFFF, 1.0f); // White

    ColorTimeline blended = t1.blended(t2, 0.5f);

    TEST_ASSERT_EQUAL(2, blended.size());
    uint16_t c = blended.getColor(0.5f);
    TEST_ASSERT_TRUE(c != 0xF800 && c != 0x07E0); // Ensure new mixed color
}

void test_color_timeline_sample() {
    ColorTimeline timeline;
    timeline.addKeyframe(0x0000, 0.0f);  // Black
    timeline.addKeyframe(0xFFFF, 1.0f);  // White

    std::vector<uint16_t> samples = timeline.sample(0.0f, 1.0f, 5);
    TEST_ASSERT_EQUAL(5, samples.size());
    TEST_ASSERT_EQUAL_UINT16(0x0000, samples.front());
    TEST_ASSERT_EQUAL_UINT16(0xFFFF, samples.back());
}

void test_color_timeline_positions_mapped() {
    ColorTimeline timeline;
    timeline.addKeyframe(0x0000, 0.0f);
    timeline.addKeyframe(0xFFFF, 1.0f);

    std::vector<float> mapped = timeline.getPositionsMapped(10.0f, 20.0f);
    TEST_ASSERT_EQUAL(2, mapped.size());
    TEST_ASSERT_EQUAL_FLOAT(10.0f, mapped.front());
    TEST_ASSERT_EQUAL_FLOAT(20.0f, mapped.back());
}

void setUp() {}
void tearDown() {}

int runUnityTests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_color_timeline_basic);
    RUN_TEST(test_color_timeline_ordering);
    RUN_TEST(test_color_timeline_copy_and_assign);
    RUN_TEST(test_color_timeline_blend_static);
    RUN_TEST(test_color_timeline_blend_two_timelines);
    RUN_TEST(test_color_timeline_sample);
    RUN_TEST(test_color_timeline_positions_mapped);
    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnityTests();
}

void loop() {}
