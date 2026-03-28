#!/bin/bash
repo="https://github.com/Raao-tech/Game_Violeta_Rafael.git";
adios="6";

# Revisamos la memoria de ingit
stats_file="otros/memoria_ingit.txt"
mkdir -p otros  # Crea la carpeta si no existe

if [ ! -f "$stats_file" ]; then
    echo -e "Aperturas\t0\nUltimo_name\tNAN\nUltimo_user\tNAN\nultima_fecha\t$(date +'%H\t%M\t%d\t%m\t%Y')" > "$stats_file"
fi

# Leer datos con un valor por defecto si falla awk
aperturas=$(awk '/Aperturas/ {print $2}' "$stats_file")
aperturas=${aperturas:-0} # Si está vacío, pon 0

ultimo_name=$(awk '/Ultimo_name/ {print $2}' "$stats_file")
ultimo_name=${ultimo_name:-"NAN"}

ultimo_user=$(awk '/Ultimo_user/ {print $2}' "$stats_file")
ultimo_user=${ultimo_user:-"NAN"}

if [ "$aperturas" -eq 0 ]; then
    echo -e "Hola! Soy tu asistente de Pre_prog, llamame Ingit.\n"
    read -p "Veo que es tu primera vez por aquí. ¿Quién eres? responde: " name
    read -p "Vale, ahora necesito conocer tu USERNAME de GITHUB porque sin eso no se para que estas aqui: " username
    # Actualizamos el archivo: Aperturas pasa a 1 y guardamos el nombre
    sed -i "s/Aperturas.*/Aperturas\t1/" "$stats_file"
    sed -i "s/Ultimo_name.*/Ultimo_name\t$name/" "$stats_file"
    sed -i "s/Ultimo_user.*/Ultimo_user\t$username/" "$stats_file"

    sleep 1
    echo -e "¿En que te puedo ayudar hoy?\n"
    sleep 1
    echo -e "1. iniciar mi repo desde una compu de la uni\n";
    echo -e "2. iniciar mi repo desde mi compu (no funciona)";
else
    name=$ultimo_name
    echo -e "¡Hola de nuevo, ${name}! Soy Ingit.\n"
    sleep 1
    echo -e "¿En que te puedo ayudar esta vez? \n"
    sleep 1
    echo -e "\n-----------Opciones de administracion------------\n";
    echo -e "3. Guarda y manda mis cambios\n";
    echo -e "4. Visualizar Ramas (no funciona)\n";
    echo -e "5. Estado del proyecto (no funciona)\n";
fi

echo -e "${adios}. Adios Ingit (No usar)\n";


play=1;
sleep 1
while [ $play -eq 1 ]; do
    read -p "Hare la opcion: " opcion;
    if [ "$opcion" == "${adios}" ]; then
        echo -e "Marchando ${name}!!"
        sleep 1
        echo -e "Dame un chance que estoy limpiando toda nuestra conversacion"
        sleep 2.5
        clear
        sleep 1
        echo -e "Todo listo mi brother"
        sleep 1
        echo -e "Por cierto, nadie debe de enterarse de esta conversacion"
        sleep 0.5 
        echo -e "¿Capichi mi pana?"
        sleep 1.5
        echo -e "... Mas te vale"
        sleep 3
        clear
        echo -e "Dicho eso, vente cuando quieras!!"
        sleep 2
        clear
        play=0;

        # Guardar fecha actual y resetear aperturas a 0
        fecha_actual=$(date +'%H\t%M\t%d\t%m\t%Y')
        sed -i "s/Aperturas.*/Aperturas\t0/" "$stats_file"
        sed -i "s/ultima_fecha.*/ultima_fecha\t$fecha_actual/" "$stats_file"

        #Limpiamos el config gnereal del repo (talk vez no sea lo mejor si es tu compu, pero por ahora lo dejaremos asi)
        echo -e "Por cierto, deberias asegurarte de volver a iniciar tus crendeciales si queires trabajar en otro repo, o usame para tus proyectos personales\n"
        echo -e "Soy una gran asistente jejeje... por favor, aca no me pagan, me estan explotando\n"
        echo -e "Bueno, continaure borrando cosas, ya estas avisado... llamame al +58 555 555 55 para pdoer ayudarte luego jejeje\n"
        sleep 3
        git config --global --unset user.name
        git config --global --unset user.email
        git remote remove origin

        exit 0;
    elif (( opcion < 1 || opcion >= adios )); then
        echo -e "${name} Has introducido una opcion invalida.... Dios, lo que me toca vivir\n";
    else 
        echo -e "Ok ${name}! En camino...\n";
        play=0;
    fi
done

sleep 1.5
clear
if [ "$opcion" == 1 ]; then
    echo -e "Antes de empezar, voy a crear un repo, se supone que has descargado esto desde un .zip"
    sleep 0.5
    read -p "¿Correcto? (responde  si, yes, s, y, todo lo demas sera un NO): " respuesta;

    if [[ ! "$respuesta" =~ ^(si|yes|s|y|S|correct)$ ]]; then
        sleep 0.3
        echo -e "Pues ¿que haces aqui? Deja de joder"
        exit 0
    fi
    clear;
    sleep 1
    echo -e "Iniciando repo en el directorio actual"
    git init
    git config --global init.defaultBranch main
    git branch -m main

    sleep 0.35
    read -p "cual es tu USERNAME de github? : " username
    read -p "cual es tu EMAIL de github? : " email
    read -p "Cual es tu TOKEN de uni? : " token
    
    echo -e "Configurando credenciales temporales..."
    git config --global user.email "$email"
    git config --global user.name "$username"

    echo -e "Estableciendo el enlace HTTPS al repo"
    
    url_con_token="https://${username}:${token}@github.com/Raao-tech/Game_Violeta_Rafael.git"
    
    git remote remove origin 2>/dev/null
    git remote add origin "$url_con_token"

    echo -e "Pidiendo datos al repo...\n"
    git fetch origin
    
    echo -e "Sincronizando con la nube (borrando basura local)...\n"
    git reset --hard origin/main
    
    # Esto conecta tu rama local 'main' con la de GitHub para siempre
    git branch --set-upstream-to=origin/main main

    # Guardar fecha actual y resetear aperturas a 0
    sed -i "s/Aperturas.*/Aperturas\t0/" "$stats_file"
    git add otros/memoria_ingit.txt
    fecha_actual=$(date +'%H\t%M\t%d\t%m\t%Y')
    git commit -m "Ingreso de ${username} el dia ${fecha_actual}"
    echo -e "¡Listo! El repo está vinculado y al día.\n"
    
    sleep 1.5
    echo -e "Estado actual del repo:"
    git status
    ls -la
elif [ "$opcion" == 3 ]; then
    read -p "¿Qué cambios has hecho? (Mensaje para el commit): " cambios;

    echo -e "OK. Comenzaremos viendo si hay errores de compatibilidad...\n"

    # 1. Intentamos traer lo de la nube
    git fetch origin main

    # 2. Intentamos un pull. Si falla, hay conflictos.
    if ! git pull origin main --rebase; then
        echo -e "¡HOUSTON, TENEMOS UN CONFLICTO! 
        Alguien ha tocado las mismas líneas que tú. 
        Abre los archivos marcados, busca las marcas '<<<<<<', límpialas y guarda."
        
        # Aquí el script se detiene para que el humano arregle el código
        read -p "Presiona ENTER cuando hayas resuelto los conflictos en el código..." listo
        
        git add .
        git rebase --continue
    fi

    # 3. Una vez limpio, mandamos 
    git add .
    git commit -m "$cambios"
    git push origin main

    fecha_actual=$(date +'%H\t%M\t%d\t%m\t%Y')
    sed -i "s/ultima_fecha.*/ultima_fecha\t$fecha_actual/" "$stats_file"
    echo -e "Vale. Ya estaría, registro de fecha actualizado. Vuelve cuando quieras.\n" echo -e "Vale. Ya estaria, si necesitas algo mas, vuelve a llamarme\n"
    sleep 1.5
    clear
        
fi



