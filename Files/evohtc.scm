;;; SCHEME FILE FOR CALCULATING AND APPLYING HTC EVERY TIME STEP 
;;; AUTHOR: Mikko Karkkainen

;;; Run DEFINE_ON_DEMAND(evolve_htc) to calculate the value of wall thickness and assign it in the variable 'thickness
(%udf-on-demand 'evolve_htc::libudf) 

;;; Calculate volume average of solid fraction of fluid zone (NEEDS CORRECT FLUID ZONE ID VALUE)
(%fill-node-values 'liquid-fraction)
(rp-var-define 'fl (%volume-integrals 'volume-average '(5) "liquid fraction") 'real #f)
;;; Run the menu command to apply the initial wall thickness to the thin-wall (needs to be performed every time step)
(ti-menu-load-string (string-append "def bc wall" " " (number->string (%rpgetvar 'wall_id)) " " (number->string (%rpgetvar 'thickness)) ",,,,,,,"))
