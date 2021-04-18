#include <include/VsiTruthHelper.h>

#include <xAODTruth/TruthParticle.h>

namespace VsiTruthHelper {
  
  std::map<std::string, bool (*)(const xAOD::TruthVertex*)> pidFuncs;
  
  auto registerFunc = []() {
    pidFuncs["Rhadron"]            = VsiTruthHelper::selectRhadron;
    pidFuncs["N1"]                 = VsiTruthHelper::selectN1;
	pidFuncs["Higgs"]              = VsiTruthHelper::selectHiggs;
    pidFuncs["Stop1"]              = VsiTruthHelper::selectStop1;
    pidFuncs["InvisibleFromHiggs"] = VsiTruthHelper::selectInvisibleFromHiggs;
    pidFuncs["DarkPhoton"]         = VsiTruthHelper::selectDarkPhoton;
    pidFuncs["Bmeson"]             = VsiTruthHelper::selectBmeson;
    pidFuncs["HadInt"]             = VsiTruthHelper::selectHadInt;
    pidFuncs["Kshort"]             = VsiTruthHelper::selectKshort;
    pidFuncs["HNL"]                = VsiTruthHelper::selectHNL;
    return true;
  };
  
  bool registerPidFuncs = registerFunc();
  
  bool selectRhadron(const xAOD::TruthVertex* truthVertex ) {
    
    if( truthVertex->nIncomingParticles() != 1 )                      return false;
    auto& links = truthVertex->incomingParticleLinks();
    if( links.size() != 1 ) return false;
    if( !links.at(0).isValid() ) return false;
    if( abs(truthVertex->incomingParticle(0)->pdgId()) < 1000000 )    return false;
    if( abs(truthVertex->incomingParticle(0)->pdgId()) > 1000000000 ) return false; // Nuclear codes, e.g. deuteron
    
    // check if the R-hadron daughter exists
    bool hasNeutralino = false;
    for( unsigned ip = 0; ip < truthVertex->nOutgoingParticles(); ip++ ) {
      auto* p = truthVertex->outgoingParticle(ip);
      if( abs( p->pdgId() ) == 1000022 ) {
        hasNeutralino = true;
        break;
      }
    }
    if( !hasNeutralino ) {
      return false;
    }
    return true;
  }
  
  
  bool selectN1(const xAOD::TruthVertex* truthVertex ) {
    
    if( truthVertex->nIncomingParticles() != 1 )                      return false;
    auto& links = truthVertex->incomingParticleLinks();
    if( links.size() != 1 ) return false;
    if( !links.at(0).isValid() ) return false;
    if( abs(truthVertex->incomingParticle(0)->pdgId()) != 1000022 )    return false;
    return true;
  }
 
  bool selectHiggs(const xAOD::TruthVertex* truthVertex ) {
    
    if( truthVertex->nIncomingParticles() != 1 )                      return false;
    auto& links = truthVertex->incomingParticleLinks();
    if( links.size() != 1 ) return false;
    if( !links.at(0).isValid() ) return false;
    if( abs(truthVertex->incomingParticle(0)->pdgId()) != 25 )        return false;
    return true;
  }
  
  bool selectStop1(const xAOD::TruthVertex* truthVertex ) {
    
    if( truthVertex->nIncomingParticles() != 1 )                      return false;
    auto& links = truthVertex->incomingParticleLinks();
    if( links.size() != 1 ) return false;
    if( !links.at(0).isValid() ) return false;
    if( abs(truthVertex->incomingParticle(0)->pdgId()) != 1000006 )   return false; // 1000006 == stop-1
    
    bool hasSparticle = false;
    for( unsigned ip = 0; ip < truthVertex->nOutgoingParticles(); ip++ ) {
      auto* p = truthVertex->outgoingParticle(ip);
      if( !p ) continue;
      if( abs( p->pdgId() ) > 1000000 ) {
        hasSparticle = true;
      }
    }
    if( hasSparticle ) return false;
    
    return true;
  }
  

  bool selectInvisibleFromHiggs(const xAOD::TruthVertex* truthVertex ) {
    
    if( truthVertex->nIncomingParticles() != 1 )                      return false;
    auto& links = truthVertex->incomingParticleLinks();
    if( links.size() != 1 ) return false;
    if( !links.at(0).isValid() ) return false;
    if( abs(truthVertex->incomingParticle(0)->pdgId()) != 36 )        return false; // 36 == invisible
    
    return true;
  }
  
  
  bool selectBmeson(const xAOD::TruthVertex* truthVertex ) {
    if( truthVertex->perp() < 1.0 )                                   return false;
    if( truthVertex->nIncomingParticles() != 1 )                      return false;
    auto& links = truthVertex->incomingParticleLinks();
    if( links.size() != 1 ) return false;
    if( !links.at(0).isValid() ) return false;
    auto* particle = *( links.at(0) );
    if( !particle )                                                   return false;
    auto pdgId = abs(particle->pdgId());
    if( ! (pdgId>500 && pdgId<600) )                                  return false;
    
    return true;
  }
  
  
  bool selectDarkPhoton(const xAOD::TruthVertex* truthVertex ) {
    if( truthVertex->perp() < 0.1 )                                   return false;
    if( truthVertex->nIncomingParticles() != 1 )                      return false;
    auto& links = truthVertex->incomingParticleLinks();
    if( links.size() != 1 ) return false;
    if( !links.at(0).isValid() ) return false;
    if( abs(truthVertex->incomingParticle(0)->pdgId()) != 4900111 )   return false;
    
    return true;
  }
  
  bool selectKshort(const xAOD::TruthVertex* truthVertex ) {
    if( truthVertex->nIncomingParticles() != 1 )                      return false;
    auto& links = truthVertex->incomingParticleLinks();
    if( links.size() != 1 ) return false;
    if( !links.at(0).isValid() ) return false;
    if( abs(truthVertex->incomingParticle(0)->pdgId()) != 310 )       return false;
    return true;
  }
  
  
  bool selectHadInt(const xAOD::TruthVertex* truthVertex ) {
    if( truthVertex->perp() < 20.0 )                                  return false;
    if( truthVertex->nIncomingParticles() != 1 )                      return false;
    auto& links = truthVertex->incomingParticleLinks();
    if( links.size() != 1 ) return false;
    if( !links.at(0).isValid() ) return false;
    
    auto* parent = truthVertex->incomingParticle(0);
    if( !parent ) return false;
    if( parent->isLepton() )                                          return false;
    
    TLorentzVector p4sum_in;
    TLorentzVector p4sum_out;
    for( unsigned ip = 0; ip < truthVertex->nIncomingParticles(); ip++ ) {
      auto* particle = truthVertex->incomingParticle(ip);
      TLorentzVector p4; p4.SetPtEtaPhiM( particle->pt(), particle->eta(), particle->phi(), particle->m() );
      p4sum_in += p4;
    }
    for( unsigned ip = 0; ip < truthVertex->nOutgoingParticles(); ip++ ) {
      auto* particle = truthVertex->outgoingParticle(ip);
      if( !particle ) continue;
      TLorentzVector p4; p4.SetPtEtaPhiM( particle->pt(), particle->eta(), particle->phi(), particle->m() );
      p4sum_out += p4;
    }
    if( p4sum_out.E() - p4sum_in.E() < 100. )                         return false;
    return true;
  }
  
  
//++++++++++++++++++++++++++++++++++++++++++++++++
   
  bool selectHNL(const xAOD::TruthVertex* truthVertex ) {

    if( truthVertex->nIncomingParticles() != 1 )                      return false;
    auto& links = truthVertex->incomingParticleLinks();
    if( links.size() != 1 ) return false;
    if( !links.at(0).isValid() ) return false;
    if( abs(truthVertex->incomingParticle(0)->pdgId()) != 50 )        return false; // 50 == Heavy Neutral Lepton (HNL)

    /*
    if( truthVertex->nIncomingParticles() == 1 && abs(truthVertex->incomingParticle(0)->pdgId()) == 50 ){
       for( unsigned ip = 0; ip < truthVertex->nOutgoingParticles(); ip++ ) {
           auto* p = truthVertex->outgoingParticle(ip);
           if( !p ) continue;
          }

     } 
   */

   // For the daughters which are HNL only
   ///std::cout <<  "--------------------------"  << std::endl;
    ///if( truthVertex->nIncomingParticles() == 1 && abs(truthVertex->incomingParticle(0)->pdgId()) == 50 ){
       ///for( unsigned ip = 0; ip < truthVertex->nOutgoingParticles(); ip++ ) {
    
           ///auto* p = truthVertex->outgoingParticle(ip);
           ///if( !p ) continue;
           ///if ( abs(p->pdgId()) == 50 ) {
           ///std::cout <<  "PDGId of HNL daughter (" << ip << "): " << p->pdgId() << std::endl;
           ///std::cout <<  "Status of HNL daughter: " << p->status() << std::endl;
           ///}
       ///}

    ///}


    bool hasHNLsig = false;
    for( unsigned ip = 0; ip < truthVertex->nOutgoingParticles(); ip++ ) {
      auto* p = truthVertex->outgoingParticle(ip);
      if( !p ) continue;
      if( abs( p->pdgId() ) == 13 || abs( p->pdgId() ) == 11 ) { // checking for muons or electrons
        hasHNLsig = true;
      }
    }
    if( hasHNLsig ) return true;

    return false;
  }


//++++++++++++++++++++++++++++++++++++++++++++++++
 
  const xAOD::TruthParticle* getTruthParticle       ( const xAOD::TrackParticle* trk ) {
    
    typedef ElementLink<xAOD::TruthParticleContainer> truthLink;
    const xAOD::TruthParticle *truth { nullptr };
      
    if( trk->isAvailable< truthLink >("truthParticleLink") ) {
      try {
        const truthLink& link = trk->auxdataConst< truthLink >("truthParticleLink");
        truth = *link;
      } catch(...) {}
    }
    return truth;
    
  }
  
  
  const xAOD::TruthParticle* getParentTruthParticle ( const xAOD::TrackParticle* trk ) {
    const auto* truthParticle = getTruthParticle( trk );
      
    if( !truthParticle ) return nullptr;
    
    return truthParticle->parent(0);
  }
  
  
  
  const xAOD::TruthVertex*   getProdVtx             ( const xAOD::TrackParticle* trk ) {
    const auto* truthParticle = getTruthParticle( trk );
      
    if( !truthParticle ) return nullptr;
      
    return truthParticle->prodVtx();
  }
  
  
  
  bool isReconstructed( const xAOD::TruthParticle* particle, const xAOD::TrackParticleContainer* tracks ) {
    
    bool flag { false };
    
    if( fabs( particle->charge() ) > 0. ) {
      for( auto* trk : *tracks ) {
        const auto* truth = VsiTruthHelper::getTruthParticle( trk );
        if( particle == truth ) {
          flag = true;
          break;
        }
      }
    }
    return flag;
  }
  

  bool vertexInFiducialVolume(const xAOD::TruthVertex* signalTruthVertex){

    if( signalTruthVertex->perp() > 563. )       return false;
    if( fabs( signalTruthVertex->z() ) > 2720. ) return false;

    unsigned int nC = 0;
    for( size_t itrk=0; itrk<signalTruthVertex->nOutgoingParticles(); itrk++) {
      const auto* particle = signalTruthVertex->outgoingParticle( itrk );
      if( !particle->isCharged()    ) continue;
      if( particle->pt() < 1000.0    ) continue;
      nC++;
    }

    if( nC<2 ) return false;
    return true;

  }
  
  
  void countReconstructibleDescendentParticles(const xAOD::TruthVertex* signalTruthVertex,
                                               std::set<const xAOD::TruthParticle*>& set,
                                               const bool doRecursive,
                                               const double distanceCutoff,
                                               const double pTthr)
  {
    
    if( signalTruthVertex->perp() > 563. )       return;
    if( fabs( signalTruthVertex->z() ) > 2720. ) return;
  
    for( size_t itrk=0; itrk<signalTruthVertex->nOutgoingParticles(); itrk++) {
      
      const auto* particle = signalTruthVertex->outgoingParticle( itrk );
      
      if( !particle ) continue;
      
      // Recursively add descendents
      if( particle->hasDecayVtx() ) {
        
        TVector3 decayPos( particle->decayVtx()->x(), particle->decayVtx()->y(), particle->decayVtx()->z() );
        TVector3 prodPos ( particle->prodVtx()->x(),  particle->prodVtx()->y(),  particle->prodVtx()->z()  );
        
        auto isInside  = []( TVector3& v ) { return ( v.Perp() < 300. && fabs( v.z() ) < 1500. ); };
        auto isOutside = []( TVector3& v ) { return ( v.Perp() > 563. || fabs( v.z() ) > 2720. ); };
        
        const auto distance = (decayPos - prodPos).Mag();
        
        if( distance < distanceCutoff ) {
          
          if( doRecursive ) VsiTruthHelper::countReconstructibleDescendentParticles( particle->decayVtx(), set, doRecursive, distanceCutoff, pTthr );
          
        }
        
        else if( isInside ( prodPos  )  &&
                 isOutside( decayPos )  &&
                 particle->isCharged()  &&
                 particle->pt() > pTthr    ) {
          
          set.emplace( particle );
            
        } else if( particle->isElectron() || particle->isMuon() ) {
          
          set.emplace( particle );
            
        }
        
      } else {
        
        if( !(particle->isCharged()  && particle->pt() > pTthr) ) continue;
        set.emplace( particle );
        
      }
    }
    
    return;
  }
  
  
}
