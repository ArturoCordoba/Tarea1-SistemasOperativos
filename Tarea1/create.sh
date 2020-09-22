cd /home/arturo/Documents/Repositorios/SistemasOperativos/Tarea1/ 
docker build -t op-tarea1-image .
docker run -p 9090:8080 -v /home/arturo/Desktop/dstorage/:/psot1-dstorage/ --detach --name op-tarea1-container op-tarea1-image
docker cp configuracion.config op-tarea1-container:/configuracion.config

