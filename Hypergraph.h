#pragma once
#include <cassert>
#include <list>
#include <ranges>
#include <vector>

namespace FMCut {
struct Hypergraph final {
  using NetIdx = unsigned;
  using CellIdx = unsigned;
  using Cell = std::list<NetIdx>;
  using Net = std::list<CellIdx>;

private:
  std::vector<Net> Nets;
  std::vector<Cell> Cells;
  void addNet(auto &&NetCells, NetIdx NextNID) {
    using namespace std::ranges::views;
    auto &&CIDsToAdd =
        filter(NetCells, [NextNID, &Cells = this->Cells](const auto &CID) {
          assert(CID >= 0 && CID < Cells.size());
          return Cells[CID].empty() || Cells[CID].front() != NextNID;
        });
    Nets[NextNID] = Net{CIDsToAdd.begin(), CIDsToAdd.end()};
    for (auto CID : CIDsToAdd)
      Cells[CID].emplace_front(NextNID);
  }
  void removeNets(auto &&NIDsToRemove) {
    for (auto NID : NIDsToRemove)
      for (auto CID : Nets[NID])
        Cells[CID].remove(NID);

    for (auto NID : NIDsToRemove)
      Nets.erase(Nets.begin() + NID);

    for (auto &&Cell : Cells)
      for (auto NID : Cell) {
        NetIdx Count = 0;
        for (auto NIDToRemove : NIDsToRemove)
          if (NID > NIDToRemove)
            ++Count;
        NID -= Count;
      }
  }

public:
  template <std::ranges::range Range>
  Hypergraph(decltype(Nets)::size_type NNets, decltype(Cells)::size_type NCells,
             Range &&Ns)
      : Nets(NNets), Cells(NCells) {
    NetIdx NextNID = 0;
    for (auto &&Net : Ns)
      addNet(Net, NextNID++);
    assert(NextNID == Nets.size());

    std::vector<NetIdx> NIDsToRemove;
    for (NetIdx NID = 0; NID < Nets.size(); ++NID)
      if (Nets[NID].size() == 1)
        NIDsToRemove.emplace_back(NID);
    removeNets(NIDsToRemove);
  }
  const auto &getCells() const { return Cells; }
  const auto &getNets() const { return Nets; }
};

} // namespace FMCut
