The files contained here define custom physics lists which are based on GEANT4 reference physics.
A brief description of each physics list is included below:

MyQGSP_BERT_HP_NeutronXSBias:
  - author: rivera@lanl.gov
  - based on QGSP_BERT_HP reference physics list
  - intended for neutron cross section studies
  - defines an environment variable: NEUTRON_INXS_SCALE, which defaults to 1.00, and which can
    be used to scale the neutron inelastic cross section in GEANT4 by a constant factor
  - select this physics list by setting the parameter:
      services.PhysicsList.PhysicsListName: "MyQGSP_BERT_HP_NeutronXSBias"
    within the G4 stage fhicl file

MyQGSP_BERT_ArHP:
  - author: Jingbo.Wang@sdsmt.edu et al.
  - based on QGSP_BERT_HP reference physics list
  - intended to correct the neutron capture gamma energies
  - for Ar-40, isotopes coming soon
  - select this physics list by setting the parameter:
      services.PhysicsList.PhysicsListName: "MyQGSP_BERT_ArHP"
    within the G4 stage fhicl file

MyQGSP_BERT_LASER:
  - author: rivera@lanl.gov
  - based on QGSP_BERT reference physics list
  - intended to be used to simulate an ionization (UV, 266 nm) laser with mu+ (for now)
  - Delta production, Bremms, pair production, muonNuclear processes, decays, and Coulomb scattering
    processes are disabled for mu+ (ONLY) to yield perfectly straight tracks
  - select this physics list by setting the parameter:
      services.PhysicsList.PhysicsListName: "MyQGSP_BERT_LASER"
    within the G4 stage fhicl file
