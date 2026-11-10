## Dipendenze minime da installare
gdb per debug, cmake, compiler+linker

## Dipendenze installate da CMake

nxp-mkimage di base
uuu per flash
nxp-smct per configs
typst se non gia installato

## Binari da committare nel repo
sysmgr.bin per il core m33 di boot
altre cose utili per fare la build tramite mkimage dell'immagine da flashare con uuu

## Azioni chiamabili da Makefile o Cmake
build
image
flash (uuu)
debug (gdb only)
debug-flash (uuu + gdb)
uart

docs
docs-live

## Particolarità per debugging/flashing
- siamo su wsl quindi va prima lanciato gdb server di segger da exe e poi ci si connette dall'host linux
- prima si fa il flash dell'immagine di base/init tramite uuu, poi ci si attacca tramite debugger e poi si può scrivere il resto tramite j-link flasher nel gdb server
- forse bisogna prima avviare gdb e poi uuu (o viceversa) perchè in alcuni casi che non ho ancora capito il debugger tiene in reset tutto finchè non ci si connette


## Other
- di base tutti i target per jlink sono MIMX94398_ ma il suffisso poi può essere M33_0, M33_1, M7_0, M7_1
