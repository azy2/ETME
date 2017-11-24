Extensible Text Management Engine
=================================

Major Goals
-----------
+ Fast text editing core
+ Separate the user interface from the engine so it can be easily updated or replaced
+ Block the UI as little as possible. *Never* block for more than 16ms. The editor should feel responsive at *all* times.
+ Asynchronous *living* plugin architecture. Editor should never have to restart. Plugins should not be allowed to block the editor. Language TBD.
+ Be able to write and execute code inside the editor that interfaces with the editor.
+ Clean and modular API.
