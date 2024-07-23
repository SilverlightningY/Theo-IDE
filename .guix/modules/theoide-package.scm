(define-module (theoide-package)
               #:use-module (guix)
               #:use-module (guix gexp)
               #:use-module (guix packages)
               #:use-module (guix git-download)
               #:use-module (guix build-system qt)
               #:use-module (guix licenses)
               #:use-module (gnu packages qt)
               #:use-module (gnu packages cmake)
               #:use-module (gnu packages mold)
               #:use-module (gnu packages ninja))

(define vcs-file?
  (or (git-predicate (dirname (dirname (current-source-directory))))
      (const #t)))

(define-public theoide
               (package
                 (name "TheoIDE")
                 (version "0.1.0-git")
                 (source (local-file "../../" "theoide-checkout"
                                     #:recursive? #t
                                     #:select? vcs-file?))
                 (build-system qt-build-system)
                 (arguments
                   (list
                     #:cmake cmake
                     #:tests? #f
                     #:configure-flags #~(list "-G Ninja" "-DQT_BUILD_TESTS=OFF")
                     #:phases 
                     #~(modify-phases %standard-phases
                        (replace 'build
                          (lambda* (#:key parallel-build? #:allow-other-keys)
                            (apply invoke "cmake" "--build" "."
                                   (if parallel-build?
                                       `("--parallel" ,(number->string (parallel-job-count)))
                                       '())))))))
                (inputs (list qtbase qtdeclarative))
                (native-inputs (list mold-as-ld-wrapper ninja))
                (home-page "https://github.com/Theo-IDE")
                (synopsis "IDE for concept languages such as LOOP, WHILE and GOTO")
                (description "Integrated development environment for concept
                              languages such as LOOP, WHILE and GOTO
                              programs.")
                (license gpl3+)))
 
theoide
