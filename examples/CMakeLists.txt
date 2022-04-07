install(DIRECTORY ./ DESTINATION examples
  FILES_MATCHING REGEX "\\.(fcl|gdml|xml|xsd|C|mac)$")

file(COPY . DESTINATION .
  FILES_MATCHING REGEX "\\.(fcl|gdml|xml|xsd|C|mac)$")
