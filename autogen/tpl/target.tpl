[< AutoGen5 template in >]
[< CASE (suffix) >]
[< == in >][< HEADER comment="#" >]
[< FOR paths "\n" >][< (get "name") >]=[< (get "value") >][< ENDFOR >]
mcu=[< mcu >]
arch=[< arch >]
[< (out-move "./target.autogen.in") >]
[< ESAC >]
[< define HEADER >][< comment >] This file was generated by AutoGen [< (emit autogen-version) >]
[< comment >] on [< (shell "date") >]
[< enddef >]