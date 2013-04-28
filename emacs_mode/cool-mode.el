;; define some variables
(defvar cool-mode-hook nil)

;; define map for key bindings
(defvar cool-mode-map 
  (let ((map (make-keymap)))
    (define-key map "\C-j" 'newline-and-indent)
    map)
  "Keymap for Cool major mode")

;; syntax highlighting
;; (regexp-opt '("class" "case" "else" "esac" "false" "fi"
;;   "if" "in" "inherits" "isvoid" "let" "loop"
;;   "new" "not" "of" "pool" "then" "true" "while") t)

(defconst cool-font-lock-keywords-1
  (list 
   '("\\<\\(c\\(?:ase\\|lass\\)\\|e\\(?:lse\\|sac\\)\\|f\\(?:alse\\|i\\)\\|i\\(?:nherits\\|svoid\\|[fn]\\)\\|l\\(?:et\\|oop\\)\\|n\\(?:ew\\|ot\\)\\|of\\|pool\\|t\\(?:hen\\|rue\\)\\|while\\)\\>" . font-lock-builtin-face)
   '("\\(\\w*\\)" . font-lock-variable-name-face))
  "Minimal highlighting expressions for COOL mode")

(defvar cool-font-lock-keywords cool-font-lock-keywords-1)

;; indentation
(defun cool-indent-line () 
  "Indent current line in cool mode"
  (interactive)
  (if (= (point) (point-at-bol))
      (insert-tab)
    (message "previous indent %d" (cool-previous-indent))))

(defun cool-previous-indent () 
  "Return the indentation level of the prebious non blank line."
  (save-excursion
    (forward-line -1)
    (if (bobp)
	0
      (progn
	(while (and (looking-at "^[ \t]*$") (not (bobp))) 
	  (forward-line -1))
	(current-indentation)))))

;; syntax table
(defvar cool-mode-syntax-table 
  (let ((st (make-syntax-table)))
    (modify-syntax-entry ?_ "w" st)
    (modify-syntax-entry ?( ". 1" st)
    (modify-syntax-entry ?) ". 4" st)
    (modify-syntax-entry ?* ". 23" st)
    (modify-syntax-entry ?- ". 12b" st)
    (modify-syntax-entry ?\n "> b" st)
    st)
  "Syntax table for cool-mode")

(defun cool-mode () 
  "Major mode for editing Cool program language"
  (interactive)
  (kill-all-local-variables)
  (set-syntax-table cool-mode-syntax-table)
  (use-local-map cool-mode-map)
  (set (make-local-variable 'font-lock-defaults) '(cool-font-lock-keywords))
  (set (make-local-variable 'indent-line-function) 'cool-indent-line) 
  (setq major-mode 'cool-mode)
  (setq mode-name "COOL")
  (run-hooks 'cool-mode-hook))

(provide 'cool-mode)

