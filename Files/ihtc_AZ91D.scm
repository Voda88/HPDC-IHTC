;;; SCHEME FILE FOR CALCULATING INITIAL HTC for A380
;;; AUTHOR: Mikko Karkkainen

;;; Initialize variable for initial thickness and total thickness
(rp-var-define 'thickness 0 'real #f)
(rp-var-define 'total_thickness 0.002 'real #f)

;;; Initialize variable for air gap
(rp-var-define 'y_0 0 'real #f)

;;; Initialize fitting constant for evolution
(rp-var-define 'c 0.0000002 'real #f)
(rp-var-define 'k_res 1000 'real #f)

;;; Initialize material and processing parameters
(rp-var-define 'pi 3.14159265358979323846 'real #f)
(rp-var-define 'v 3 'real #f)	;; cast metal impact velocity
(rp-var-define 'wave_speed 3840.0 'real #f) ;; sonic wave speed in melt
(rp-var-define 'rho 2830.0 'real #f)	;; cast metal density
(rp-var-define 'impact_angle 0.061 'real #f); // Impact angle in radians
(rp-var-define 'k_m 29.0 'real #f) ;; mould thermal conductivity
(rp-var-define 'k_c 109.0 'real #f) ;; melt thermal conductivity
(rp-var-define 'r_sm (* 128.7 (expt 10.0 -6.0)) 'real #f) ;; Mean peak spacing
(rp-var-define 'sigma (* 0.578 (expt 10.0 -6.0)) 'real #f) ;; Mean peak height deviation
(rp-var-define 'epsilon 1.5 'real #f) ;; numerical parameter
(rp-var-define 'p_0 101325 'real #f) ;; NTP pressure
(rp-var-define 't_1 840.0 'real #f) ;; impact temperature
(rp-var-define 't_0 300.0 'real #f) ;; NTP temperature
 
;;; ;;; Initialize variable for the wall zone id and shadow wall id of the thin-wall
(rp-var-define 'wall_id 4 'real #f)
(rp-var-define 'shadow_wall_id 15 'real #f)

;;; Run DEFINE_ON_DEMAND(initial_htc) to calculate the value of wall thickness and assign it in the variable 'thickness
(%udf-on-demand 'ihtc::libudf) 

;;; Run the menu command to apply the initial wall thickness to the thin-wall
(ti-menu-load-string (string-append "def bc wall" " " (number->string (%rpgetvar 'wall_id)) " " (number->string (%rpgetvar 'thickness)) ",,,,,,,"))
