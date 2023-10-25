#include "SCEPCALSegmentation.h"

#include <climits>
#include <cmath>
#include <stdexcept>

namespace dd4hep {
namespace DDSegmentation {

/// default constructor using an encoding string
        SCEPCALSegmentation::SCEPCALSegmentation(const std::string& cellEncoding) : Segmentation(cellEncoding) {

            _type = "SCEPCALSegmentation";
            _description = "SCEPCAL segmentation based on side/eta/phi/depth/S/C";

            registerIdentifier("identifier_system", "Cell ID identifier for numSystem", fSystemId, "system");
            registerIdentifier("identifier_eta", "Cell ID identifier for numEta", fEtaId, "eta");
            registerIdentifier("identifier_phi", "Cell ID identifier for numPhi", fPhiId, "phi");
            registerIdentifier("identifier_depth", "Cell ID identifier for numDepth", fDepthId, "depth");
            registerIdentifier("identifier_sipm", "Cell ID identifier for numSipm", fSipmId, "sipm");
            registerIdentifier("identifier_IsCerenkov", "Cell ID identifier for IsCerenkov", fIsCerenkovId, "c");
            registerIdentifier("identifier_module", "Cell ID identifier for module", fModule, "module");

        }

        SCEPCALSegmentation::SCEPCALSegmentation(const BitFieldCoder* decoder) : Segmentation(decoder) {

            _type = "SCEPCALSegmentation";
            _description = "SCEPCAL segmentation based on side/eta/phi/depth/S/C";

            registerIdentifier("identifier_system", "Cell ID identifier for numSystem", fSystemId, "system");
            registerIdentifier("identifier_eta", "Cell ID identifier for Eta", fEtaId, "eta");
            registerIdentifier("identifier_phi", "Cell ID identifier for Phi", fPhiId, "phi");
            registerIdentifier("identifier_depth", "Cell ID identifier for Depth", fDepthId, "depth");
            registerIdentifier("identifier_sipm", "Cell ID identifier for Sipm", fSipmId, "sipm");
            registerIdentifier("identifier_IsCerenkov", "Cell ID identifier for IsCerenkov", fIsCerenkovId, "c");
            registerIdentifier("identifier_module", "Cell ID identifier for module", fModule, "module");

        }

        SCEPCALSegmentation::~SCEPCALSegmentation() {}

        Vector3D SCEPCALSegmentation::position(const CellID& cID) const {
          return Vector3D(0,0,0);
        };


      Vector3D SCEPCALSegmentation::myPosition(const CellID& cID) {

          int copyNum = (int)cID;

          if (fPositionOf.count(copyNum) == 0) { //Add if not found
            int system=(copyNum)&(32-1);
            //int nEta_in=(copyNum>>5)&(1024-1);
            int nEta_in=Eta(copyNum);
            int nPhi_in=(copyNum>>15)&(1024-1);
            int nDepth_in=(copyNum>>25)&(8-1);

            double EBz=2.25;
            double Rin=2.0;
            double nomfw=0.1;
            double Fdz=0.05;
            double Rdz=0.15;

            int nThetaBarrel=floor(EBz/nomfw);
            int nThetaEndcap=floor(Rin/nomfw);
            int nPhi=std::floor(2*M_PI*Rin/nomfw);

            double dTheta=(M_PI/2)/(nThetaBarrel+nThetaEndcap);
            double dPhi=2*M_PI/nPhi;

            int nTheta = nEta_in>0 ? (nThetaBarrel+nThetaEndcap)-nEta_in : -((nThetaBarrel+nThetaEndcap)+nEta_in);
            std::cout << "This is nEta_in :: " << nEta_in << std::endl;
            std::cout << "So this is nTheta :: " << nTheta << std::endl;
            double thC=nTheta*dTheta;

            std::cout << "So this is thC :: " << nTheta*dTheta << std::endl;
            double phi=nPhi_in*dPhi;

            double r0=EBz/cos(thC);

            double r1=r0+Fdz;
            double r2=r1+Rdz;

            double R=nDepth_in==1 ? (r0+r1)/2 : (r1+r2)/2;
            double x=R*sin(thC)*cos(phi);
            double y=R*sin(thC)*sin(phi);
            double z= thC>0 ? R*cos(thC):-R*cos(thC);

            
            std::cout << "And finally R :: " << R << std::endl;
            std::cout << "And finally cos :: " << cos(thC) << std::endl;
            std::cout << "And finally Z :: " << z << std::endl;
            Vector3D position(x, y, z);
            fPositionOf.emplace(copyNum,position);
          }

          return fPositionOf.at(copyNum);
        }

/*
        Vector3D SCEPCALSegmentation::localPosition(const CellID& cID) const {
            int numx = numX(cID);
            int numy = numY(cID);
            int numz = numZ(cID);
            int x_ = x(cID);
            int y_ = y(cID);
            int z_ = z(cID);

            return localPosition(numx,numy,numz,x_,y_,z_);
        }

        Vector3D SCEPCALSegmentation::localPosition(int numx, int numy, int numz, int x_, int y_, int z_) const {
            float ptX = -fGridSize*static_cast<float>(numx/2) + static_cast<float>(x_)*fGridSize + ( numx%2==0 ? fGridSize/2. : 0. );
            float ptY = -fGridSize*static_cast<float>(numy/2) + static_cast<float>(y_)*fGridSize + ( numy%2==0 ? fGridSize/2. : 0. );
            float ptZ = -fGridSize*static_cast<float>(numz/2) + static_cast<float>(z_)*fGridSize + ( numz%2==0 ? fGridSize/2. : 0. );

            return Vector3D(ptX,ptY,ptZ);
        }
*/

        CellID SCEPCALSegmentation::cellID(const Vector3D& /*localPosition*/, const Vector3D& /*globalPosition*/, const VolumeID& vID) const {

            return setCellID(System(vID), Eta(vID), Phi(vID), Depth(vID) );
        }

        VolumeID SCEPCALSegmentation::setVolumeID(int System, int Eta, int Phi, int Depth) const {
            VolumeID SystemId = static_cast<VolumeID>(System);
            VolumeID EtaId = static_cast<VolumeID>(Eta);
            VolumeID PhiId = static_cast<VolumeID>(Phi);
            VolumeID DepthId = static_cast<VolumeID>(Depth);
            VolumeID vID = 0;

            //std::cout << " In setVolumeID:: " << std::endl;
            //std::cout << " EtaID:: " << EtaId <<std::endl;
            //std::cout << " PhiID:: " << PhiId <<std::endl;
            //std::cout << " DepthID:: " << DepthId <<std::endl;


            _decoder->set(vID, fSystemId, SystemId);
            _decoder->set(vID, fEtaId, EtaId);
            _decoder->set(vID, fPhiId, PhiId);
            _decoder->set(vID, fDepthId, DepthId);

            VolumeID module = 0; // Tower
            _decoder->set(vID, fModule, module);

            return vID;
        }

        CellID SCEPCALSegmentation::setCellID(int System, int Eta, int Phi, int Depth) const {
            VolumeID SystemId = static_cast<VolumeID>(System);
            VolumeID EtaId = static_cast<VolumeID>(Eta);
            VolumeID PhiId = static_cast<VolumeID>(Phi);
            VolumeID DepthId = static_cast<VolumeID>(Depth);
            VolumeID vID = 0;

            //std::cout << " In setCellID:: " << std::endl;
            //std::cout << " EtaID:: " << EtaId <<std::endl;
            //std::cout << " PhiID:: " << PhiId <<std::endl;
            //std::cout << " DepthID:: " << DepthId <<std::endl;



            _decoder->set(vID, fSystemId, SystemId);
            _decoder->set(vID, fEtaId, EtaId);
            _decoder->set(vID, fPhiId, PhiId);
            _decoder->set(vID, fDepthId, DepthId);

            VolumeID module = 1; // Fiber, SiPM, etc.
            _decoder->set(vID, fModule, module);

            VolumeID isCeren = IsCerenkov(Eta,Phi) ? 1 : 0;
            _decoder->set(vID, fIsCerenkovId, isCeren);

            return vID;
        }

        int SCEPCALSegmentation::System(const CellID& aCellID) const {
          VolumeID System = static_cast<VolumeID>(_decoder->get(aCellID, fSystemId));
          return static_cast<int>(System);
        }

        int SCEPCALSegmentation::Eta(const CellID& aCellID) const {
            VolumeID Eta = static_cast<VolumeID>(_decoder->get(aCellID, fEtaId));
            return static_cast<int>(Eta);
        }

        int SCEPCALSegmentation::Phi(const CellID& aCellID) const {
            VolumeID Phi = static_cast<VolumeID>(_decoder->get(aCellID, fPhiId));
            return static_cast<int>(Phi);
        }

        int SCEPCALSegmentation::Depth(const CellID& aCellID) const {
            VolumeID Depth = static_cast<VolumeID>(_decoder->get(aCellID, fDepthId));
            return static_cast<int>(Depth);
        }

        int SCEPCALSegmentation::Sipm(const CellID& aCellID) const {
          VolumeID Sipm = static_cast<VolumeID>(_decoder->get(aCellID, fSipmId));
          return static_cast<int>(Sipm);
        }

        bool SCEPCALSegmentation::IsCerenkov(const CellID& aCellID) const {
            VolumeID isCeren = static_cast<VolumeID>(_decoder->get(aCellID, fIsCerenkovId));
            return static_cast<bool>(isCeren);
        }

        bool SCEPCALSegmentation::IsCerenkov(int eta, int phi) const {
          bool isCeren = false;
          if ( eta%2 == 1 ) { isCeren = !isCeren; }
          if ( phi%2 == 1 ) { isCeren = !isCeren; }
          return isCeren;
        }

        bool SCEPCALSegmentation::IsTower(const CellID& aCellID) const {
            VolumeID module = static_cast<VolumeID>(_decoder->get(aCellID, fModule));
            return module==0;
        }

        bool SCEPCALSegmentation::IsSiPM(const CellID& aCellID) const {
            VolumeID module = static_cast<VolumeID>(_decoder->get(aCellID, fModule));
            return module==1;
        }

        int SCEPCALSegmentation::getLast32bits(const CellID& aCellID) const {
            CellID aId64 = aCellID >> sizeof(int)*CHAR_BIT;
            int aId32 = (int)aId64;

            return aId32;
        }


        CellID SCEPCALSegmentation::convertLast32to64(const int aId32) const {
            CellID aId64 = (CellID)aId32;
            aId64 <<= sizeof(int)*CHAR_BIT;

            return aId64;
        }

/*        DRparamBase* SCEPCALSegmentation::setParamBase(int noEta) const {
            DRparamBase* paramBase = nullptr;

            if ( fParamEndcap->unsignedTowerNo(noEta) >= fParamBarrel->GetTotTowerNum() ) paramBase = static_cast<DRparamBase*>(fParamEndcap);
            else paramBase = static_cast<DRparamBase*>(fParamBarrel);

            if ( paramBase->GetCurrentTowerNum()==noEta ) return paramBase;

            // This should not be called while building detector geometry
            if (!paramBase->IsFinalized()) throw std::runtime_error("SCEPCALSegmentation::position should not be called while building detector geometry!");

            paramBase->SetDeltaThetaByTowerNo(noEta, fParamBarrel->GetTotTowerNum());
            paramBase->SetThetaOfCenterByTowerNo(noEta, fParamBarrel->GetTotTowerNum());
            paramBase->SetIsRHSByTowerNo(noEta);
            paramBase->SetCurrentTowerNum(noEta);
            paramBase->init();

            return paramBase;
        }*/


    }
}
