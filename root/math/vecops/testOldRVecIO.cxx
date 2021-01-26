#include <ROOT/RVec.hxx>
#include <TFile.h>
#include <TInterpreter.h>
#include <TKey.h>
#include <TSystem.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>

#include <algorithm> // std::equal
#include <array>
#include <iostream>
#include <vector>

#include <gtest/gtest.h>

const char *fgInputFile = "old_rvec.root";

TEST(OldRVecIO, ReadWithTTree)
{
   TFile f(fgInputFile);
   auto t = f.Get<TTree>("t");
   auto vb = new ROOT::RVec<bool>();
   auto vi = new ROOT::RVec<int>();
   t->SetBranchAddress("vb", &vb);
   t->SetBranchAddress("vi", &vi);
   ASSERT_GE(t->GetEntry(0ll), 0);
   EXPECT_TRUE(All(*vi == ROOT::RVec<int>{1, 2, 3}));
   EXPECT_TRUE(All(*vb == ROOT::RVec<bool>{true, false, true}));
}

TEST(RVecWriteObject, ReadObjectAny)
{
   TFile f(fgInputFile);
   auto *vi = static_cast<ROOT::RVec<int>*>(f.GetKey("vi")->ReadObjectAny(TClass::GetClass("ROOT::RVec<int>")));
   auto *vb = static_cast<ROOT::RVec<bool>*>(f.GetKey("vb")->ReadObjectAny(TClass::GetClass("ROOT::RVec<bool>")));
   ASSERT_TRUE(vi != nullptr);
   ASSERT_TRUE(vb != nullptr);

   EXPECT_TRUE(All(*vi == ROOT::RVec<int>({1, 2, 3})));
   EXPECT_TRUE(All(*vb == ROOT::RVec<bool>({true, false, true})));
}
