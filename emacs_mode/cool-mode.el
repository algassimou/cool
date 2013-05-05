;; This program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public License as
;; published by the Free Software Foundation; either version 2, or (at
;; your option) any later version.

;; This program is distributed in the hope that it will be useful, but
;; WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;; General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with this program; see the file COPYING.  If not, write to
;; the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;; Boston, MA 02110-1301, USA.

;; Thanks (https://github.com/defunkt/coffee-mode.git)

;; define some variables
(defconst cool-mode-version "0.0.1"
  "The version of `cool-mode'.")

(defgroup cool nil
  "A Cool Language major mode."
  :group 'languages)

(defcustom cool-tab-width tab-width
  "The tab width to use when indenting."
  :type 'integer
  :group 'cool)

;; define map for key bindings
(defvar cool-mode-map 
  (let ((map (make-keymap)))
    (define-key map "\C-j" 'newline-and-indent)
    map)
  "Keymap for Cool major mode")

(defcustom cool-mode-hook nil
  "Hook called by `cool-mode'."
  :type 'hook
  :group 'cool
  )

(defun cool-version () 
  "Show the `cool-mode' verison in the echo area."
  (interactive)
  (message (concat "cool-mode version " cool-mode-version)))


(defun cool-newline-and-indent () 
  "Insert a new line and indent it to the same level as the previous line."
  (interactive)

  (let ((prev-indent (current-indentation))
	(indent-next nil))
    (delete-horizontal-space t)
    (newline)
    (insert-tab (/ prev-indent cool-tab-width))
    
    (when (cool-line-wants-indent)
      (insert-tab))

    (when (cool-previous-line-is-comment)
      (insert "-- "))))

(defun cool-line-wants-indent () 
  "Return t if the current line should be indented relative to the
previous line."
  (interactive)
  (save-excursion
    (end-of-line)
    (message "char before %c" (char-before (- (point) 1)))
    (= (char-before (- (point) 1)) ?{)))

(defun cool-previous-line-is-comment ()
  "Return t if the previous line is a comment."
  (save-excursion
    (forward-line -1)
    ; (message "%S" (cool-line-is-comment))
    (cool-line-is-comment)))

(defun cool-line-is-comment ()
  "Return t if the current line is a comment."
  (save-excursion
    (backward-to-indentation 0)
    (and (= (char-after) (string-to-char "-"))
	 (= (char-after (+ (point) 1)) (string-to-char "-")))))

;; syntax highlighting
(defvar cool-keywords '("class" "case" "else" "esac" "false" "fi" "if" "in" "inherits" "isvoid" "let" "loop" "new" "not" "of" "pool" "then" "true" "while") "Cool language keywords .")

(defvar cool-keywords-regexp
  ;; "Builds a regexp out of `coffee-indenters-bol' words."
  (format "\\<%s\\>"
	  (regexp-opt cool-keywords 'words)))

;; Type
(defvar cool-namespace-regexp "\\b\\(class\\s +\\(\\S +\\)\\)\\b")
(defvar cool-inherits-regexp "\\b\\(inherits\\s +\\(\\S +\\)\\)\\b")
(defvar cool-new-regexp "\\(new\\s +\\([a-zA-Z0-9\_\-]+ *\\)\\)\\b")
(defvar cool-types-regexp "\\(:\\s *\\(\\S +\\)\\)\\b")



(defconst cool-font-lock-keywords-1
  `((,cool-new-regexp 2 font-lock-type-face)
    (,cool-namespace-regexp 2 font-lock-type-face)
    (,cool-inherits-regexp 2 font-lock-type-face)
    (,cool-keywords-regexp . font-lock-keyword-face)
    (,cool-types-regexp 2 font-lock-type-face)
    (, "\\(?:self\\)" . font-lock-constant-face)
   (,"\\(\\w*\\)" . font-lock-variable-name-face))
  "Minimal highlighting expressions for Cool mode")

;; (defconst cool-font-lock-keywords-1
;;   (list 
;;    '("\\<\\(c\\(?:ase\\|lass\\)\\|e\\(?:lse\\|sac\\)\\|f\\(?:alse\\|i\\)\\|i\\(?:nherits\\|svoid\\|[fn]\\)\\|l\\(?:et\\|oop\\)\\|n\\(?:ew\\|ot\\)\\|of\\|pool\\|t\\(?:hen\\|rue\\)\\|while\\)\\>" . font-lock-builtin-face)
;;    '("\\(\\w*\\)" . font-lock-variable-name-face))
;;   "Minimal highlighting expressions for Cool mode")

(defvar cool-font-lock-keywords cool-font-lock-keywords-1)

;; helper functions
(defvar cool-basic-types '("Bool" "Int" "IO" "String" "SELF_TYPE"))
(defvar cool-wordList (append cool-keywords cool-basic-types))
(defun cool-complete-symbol () 
  "Perform keyword completion on word before cursor."
  (interactive)
  (let ((posEnd (point))
        (meat (thing-at-point 'symbol))
        maxMatchResult)
    
    ;; when nil, set it to empty string, so user can see all lang's keywords.
    ;; if not done, try-completion on nil result lisp error.
    (when (not meat) (setq meat ""))
    (setq maxMatchResult (try-completion meat cool-wordList))
    
    (cond ((eq maxMatchResult t))
          ((null maxMatchResult)
           (message "Can't find completion for “%s”" meat)
           (ding))
          ((not (string= meat maxMatchResult))
           (delete-region (- posEnd (length meat)) posEnd)
           (insert maxMatchResult))
          (t (message "Making completion list…")
             (with-output-to-temp-buffer "*Completions*"
               (display-completion-list 
                (all-completions meat cool-wordList)
                meat))
             ((match-end N)ssage "Making completion list…%s" "done")))))


(defun cool-comment-dwim (arg)
  "Comment or uncomment current line or region in a smart way.
   For details, see `comment-dwim'." 
  (interactive "*P")
  (require 'newcomment)
  (message "dwwwwwwwwwwwwwwwwwwww")
  (let ((deactivate-mark nil) (comment-start "--") (comment-end ""))
    (comment-dwim arg)))


;; indentation
;; (defun cool-indent-line () 
;;   "Indent current line in cool mode"
;;   (interactive)
;;   (if (= (point) (point-at-bol))
;;       (insert-tab)
;;     (save-excursion
;;       (let ((prev-indent (cool-previous-indent))
;; 	    (cur-indent  (current-indentation)))
;; 	(beginning-of-line)
;; 	(insert-tab)

;; 	(when (= (point-at-bol) (point))
;; 	  (forward-char cool-tab-width))

;; 	;; We're too far, remove all indentation.
;;         (when (> (- (current-indentation) prev-indent) cool-tab-width)
;;           (backward-to-indentation 0)
;;           (delete-region (point-at-bol) (point)))

;; 	(message "previous indent %d current indent %d w %d" 
;; 		 prev-indent cur-indent cool-tab-width)))))
(defun cool-indent-line () 
  "Indent current line in cool mode"
  (interactive)
  (beginning-of-line)
  (if (bobp)
      (indent-line-to 0)
    (let ((not-indented t) (cur-indent))
      nil)))

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

(defvar cool-mode-map (make-sparse-keymap) "Key map for Cool mode.")
(define-key cool-mode-map [remap newline-and-indent] 'cool-newline-and-indent)
(define-key cool-mode-map [remap comment-dwim] 'cool-comment-dwim)
(define-key cool-mode-map (kbd "M-TAB") 'cool-complete-symbol)
;; (define-key cool-mode-map "\C-cc" 'cool-comment-dwim) 

(easy-menu-define coffee-mode-menu cool-mode-map
  "Menu for Cool mode"
  '("Cool"
    ["Version" cool-version]
    ))

(defun cool-mode () 
  "Major mode for editing Cool program language"
  (interactive)
  (kill-all-local-variables)
  (set-syntax-table cool-mode-syntax-table)
  (use-local-map cool-mode-map)
  (set (make-local-variable 'font-lock-defaults) '(cool-font-lock-keywords))
  (set (make-local-variable 'indent-line-function) 'cool-indent-line) 
  (set (make-local-variable 'comment-start) "--") 
  (set (make-local-variable 'tab-width) cool-tab-width)
  (setq major-mode 'cool-mode)
  (setq mode-name "Cool")
;;  (message "%s" cool-mode-map)
  (use-local-map cool-mode-map)
  (run-hooks 'cool-mode-hook))

(provide 'cool-mode)

(add-to-list 'auto-mode-alist '("\\.cl\\'" . cool-mode))
