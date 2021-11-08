
#include <TRootIOCtor.h>

#include "artg4tk/G4PhysModelParamStudy/AnalysisProducts/RecordChi2.hh"

#include <cassert>

#include <iostream>

InfoChi2::InfoChi2(const int rid, const double chi2, const double ndf)
  : TNamed(), fExpRecID(rid), fChi2(chi2), fNDF(ndf), fWeight(1.)
{
  fChi2MCBin.clear();
}

InfoChi2::InfoChi2(const int rid, const double chi2, const double ndf, const double wt)
  : TNamed(), fExpRecID(rid), fChi2(chi2), fNDF(ndf), fWeight(wt)
{
  fChi2MCBin.clear();
}

InfoChi2::InfoChi2(const InfoChi2& rhs) : TNamed(rhs)
{

  fExpRecID = rhs.fExpRecID;
  fChi2 = rhs.fChi2;
  fNDF = rhs.fNDF;
  fWeight = rhs.fWeight;
  fChi2MCBin = rhs.fChi2MCBin;
}

InfoChi2::InfoChi2(TRootIOCtor*) : TNamed(), fExpRecID(0), fChi2(0.), fNDF(0.), fWeight(1.) {}

bool
InfoChi2::operator==(const InfoChi2& rhs) const
{

  return (fExpRecID == rhs.fExpRecID && fabs(fChi2 - rhs.fChi2) < 1.e-10 &&
          fabs(fNDF - rhs.fNDF) < 1.e-10 && fabs(fWeight - rhs.fWeight) < 1.e-10);
  // skip checking on fChi2MCBin for now...
}

void
InfoChi2::Print(Option_t*) const
{

  std::cout << " OBJ: InfoChi2"
            << "\t" << GetName() << "\t" << GetTitle() << std::endl;
  std::cout << " DoSSiER Record ID = " << fExpRecID << " Chi2 = " << fChi2 << " NDF = " << fNDF
            << " weight = " << fWeight << std::endl;
  if (fChi2MCBin.size() > 0) {
    std::cout << " Contributions from MC bins: " << std::endl;
    for (size_t i = 0; i < fChi2MCBin.size(); ++i) {
      std::cout << " bin = " << fChi2MCBin[i].first << " contrib.chi2 = " << fChi2MCBin[i].second
                << std::endl;
    }
  }

  return;
}

RecordChi2::RecordChi2(const RecordChi2& rhs) : TNamed(rhs)
{

  fChi2Rec = rhs.fChi2Rec;
}

RecordChi2::RecordChi2(TRootIOCtor*) : TNamed() {}

bool
RecordChi2::InsertRecord(const int rid, const double chi2, const double ndf, const double wt)
{

  InfoChi2 info(rid, chi2, ndf, wt);

  for (size_t i = 0; i < fChi2Rec.size(); ++i) {
    if (fChi2Rec[i] == info) {
      std::cout << " WARNING: duplicate InfoChi2 found in the list at position " << i << std::endl;
      info.Print();
      std::cout << " This record will NOT be inserted; bail out (return) " << std::endl;
      return false;
    }
  }

  fChi2Rec.push_back(info);

  return true;
}

bool
RecordChi2::AddMCBin2Record(const int ir, const int ib, const double dchi2)
{

  if (ir >= GetNRecords()) {
    std::cout << " WARNING: invalid record id = " << ir << "; bail out (return) " << std::endl;
    return false;
  }

  fChi2Rec[ir].AddChi2MCBin(ib, dchi2);

  return true;
}
double
RecordChi2::GetSumChi2() const
{

  double sum = 0.;

  for (size_t i = 0; i < fChi2Rec.size(); ++i) {
    sum += fChi2Rec[i].GetChi2();
  }

  return sum;
}

double
RecordChi2::GetSumNDF() const
{

  double sum = 0.;

  for (size_t i = 0; i < fChi2Rec.size(); ++i) {
    sum += fChi2Rec[i].GetNDF();
  }

  return sum;
}

void
RecordChi2::Print(Option_t*) const
{

  // FIXME !!! Implement later !
  std::cout << " OBJ: RecordChi2"
            << "\t" << GetName() << "\t" << GetTitle() << std::endl;
  std::cout << " Has size = " << fChi2Rec.size()
            << " and contains the following elements : " << std::endl;
  for (size_t i = 0; i < fChi2Rec.size(); ++i) {
    fChi2Rec[i].Print();
  }

  return;
}

bool
RecordChi2::operator==(const RecordChi2& rhs) const
{

  return (fChi2Rec == rhs.fChi2Rec);
}
