#include <catch2/catch.hpp>
#include <dsp56kEmu/jitunittests.h>

TEST_CASE("JitUnittests", "[JitUnittests]" ) {
    dsp56k::JitUnittests();
    // This should be expanded out with SECTION blocks. with one section test.
    // https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md#test-cases-and-sections
}

