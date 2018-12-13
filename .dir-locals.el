;;; Directory Local Variables
;;; For more information see (info "(emacs) Directory Variables")

((c++-mode
  ;; (flycheck-clang-include-path ,(substitute-in-file-name "$HOME/Documents/mastermind/include/"))
  ;; (flycheck-gcc-include-path ,(substitute-in-file-name "$HOME/Documents/mastermind/include/"))
  (flycheck-clang-include-path "../include/")
  (flycheck-gcc-include-path "../include/")
  (flycheck-disabled-checkers c/c++-cppcheck) ;error with openmpi
  (flycheck-c/c++-gcc-executable . "/usr/bin/mpic++")
  (flycheck-c/c++-clang-executable . "/usr/bin/mpic++")))
