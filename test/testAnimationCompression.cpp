#include "TestPrologue.h"
#include <cal3d/coretrack.h>
#include <cal3d/corekeyframe.h>

TEST(empty_track_compresses_to_empty_track) {
    CalCoreTrack t(0);
    CalCoreTrackPtr p = t.compress(0.1, 0.1, 0);
    CHECK_EQUAL(0, p->getCoreKeyframeCount());
}

TEST(one_frame_track_compresses_to_itself) {
    CalCoreTrack t(0);
    t.addCoreKeyframe(new CalCoreKeyframe(0, CalVector(), CalQuaternion()));
    CalCoreTrackPtr p = t.compress(0.1, 0.1, 0);
    CHECK_EQUAL(1, p->getCoreKeyframeCount());
}

TEST(four_frames_are_merged_if_identical) {
    CalCoreTrack t(0);
    t.addCoreKeyframe(new CalCoreKeyframe(0, CalVector(), CalQuaternion()));
    t.addCoreKeyframe(new CalCoreKeyframe(0, CalVector(), CalQuaternion()));
    t.addCoreKeyframe(new CalCoreKeyframe(0, CalVector(), CalQuaternion()));
    t.addCoreKeyframe(new CalCoreKeyframe(0, CalVector(), CalQuaternion()));

    CalCoreTrackPtr p = t.compress(0.1, 0.1, 0);
    CHECK_EQUAL(2, p->getCoreKeyframeCount());
}
