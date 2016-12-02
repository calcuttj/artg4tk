#ifndef artg4tk_RecordChi2_H
#define artg4tk_RecordChi2_H 1

// #include <string>
#include <vector>
#include <TNamed.h>

// NO namespace !!!

class InfoChi2 : public TNamed {

   public:
   
      InfoChi2() : TNamed(), fExpRecID(0), fChi2(0.), fNDF(0.) {}
      InfoChi2( const int, const double, const double );
      InfoChi2( const InfoChi2& );
      InfoChi2( TRootIOCtor* );
      
      ~InfoChi2() {}
      
      void SetExpRecID( const int rid ) { fExpRecID=rid; return; }
      void SetChi2( const double chi2 ) { fChi2=chi2; return; }
      void SetNDF( const double ndf )   { fNDF=ndf; return; }
      
      int    GetExpRecID() const { return fExpRecID; }
      double GetChi2()     const { return fChi2; }
      double GetNDF()      const { return fNDF; }
     
      virtual void Print( Option_t* opt="" ) const;
      
      bool operator==( const InfoChi2& ) const;

   private:

      // data members
      //
      // std::string fSecPartName;
      // int         fSecPartID;
      int    fExpRecID;
      double fChi2;
      double fNDF;


ClassDef(InfoChi2,10)

}; 

class RecordChi2 : public TNamed {


   public:
   
      RecordChi2() : TNamed() {}
      RecordChi2( const RecordChi2& );
      RecordChi2( TRootIOCtor* );
      
      ~RecordChi2() {}
      
      bool InsertRecord( const int, const double, const double );
      
      const std::vector<InfoChi2>& GetAllRecords()    const { return fChi2Rec; }
      int                          GetNRecords()      const { return fChi2Rec.size(); }
      const InfoChi2&              GetRecord( const int i ) const { return fChi2Rec[i]; } 
      double                       GetSumChi2()       const;
      double                       GetSumNDF()        const; 
      
      virtual void Print( Option_t* opt="" ) const;
      
      bool operator==( const RecordChi2& ) const;

   private:
   
      // data membsers
      //
      std::vector<InfoChi2> fChi2Rec;
   

// this is mandatory because it derives from TObject (via TNamed)
//
ClassDef(RecordChi2,10)

};

#endif
