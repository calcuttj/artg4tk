install(DIRECTORY ./ DESTINATION macros
  REGEX "(~[0-9]*|\\.bak|\\.old|\\.orig|\\.rej)$" EXCLUDE)
file(COPY . DESTINATION .
  REGEX "(~[0-9]*|\\.bak|\\.old|\\.orig|\\.rej)$" EXCLUDE)
