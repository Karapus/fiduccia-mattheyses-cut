#include "Hypergraph.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {
using namespace FMCut;
using namespace testing;
TEST(Construct, 1Net1Cell) {
  Hypergraph HG(1, 1, std::vector<std::vector<Hypergraph::CellIdx>>{{0}});
  EXPECT_THAT(HG.getNets(), IsEmpty());
  EXPECT_THAT(HG.getCells(), ElementsAre(IsEmpty()));
}
TEST(Construct, 1Net2Cells) {
  Hypergraph HG(1, 2, std::vector<std::vector<Hypergraph::CellIdx>>{{0, 1}});
  EXPECT_THAT(HG.getNets(), ElementsAre(ElementsAre(Eq(0), Eq(1))));
  EXPECT_THAT(HG.getCells(),
              ElementsAre(ElementsAre(Eq(0)), ElementsAre(Eq(0))));
}
TEST(Construct, 2Nets2Cells) {
  Hypergraph HG(2, 2, std::vector<std::vector<Hypergraph::CellIdx>>{{0}, {1}});
  EXPECT_THAT(HG.getNets(), IsEmpty());
  EXPECT_THAT(HG.getCells(), ElementsAre(IsEmpty(), IsEmpty()));
}
TEST(Construct, 2Nets3Cells) {
  Hypergraph HG(2, 3,
                std::vector<std::vector<Hypergraph::CellIdx>>{{0, 1}, {1, 2}});
  EXPECT_THAT(HG.getNets(), ElementsAre(ElementsAre(Eq(0), Eq(1)),
                                        ElementsAre(Eq(1), Eq(2))));
  EXPECT_THAT(HG.getCells(), ElementsAre(ElementsAre(Eq(0)),
                                         WhenSorted(ElementsAre(Eq(0), Eq(1))),
                                         ElementsAre(Eq(1))));
}
} // namespace
