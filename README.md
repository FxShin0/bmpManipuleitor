# BMP Manipuleitor

**Tópicos de Programación - Lunes 2C 2024**

---

## Descripción del Proyecto

**BMP Manipuleitor** es una herramienta para manipular imágenes en formato BMP. Utiliza conceptos avanzados como memoria dinámica y punteros a funciones para implementar una variedad de filtros y funcionalidades. 

---

## Características

- **Memoria Dinámica:** Todas las matrices de datos de las imágenes se generan de forma genérica utilizando memoria dinámica.
- **Punteros a Funciones:** Se emplean punteros a funciones para evitar redundancia en el código y generalizar los filtros procesadores.
**Funciones Procesadoras**

1. **`procesarImagenesFiltroArg1Met`:**
   - **Filtros:** recortar, achicar.
   - **Detalles:** Modifican metadatos y requieren un porcentaje como parámetro.

2. **`procesarImagenesFiltroArg0`:**
   - **Filtros:**
     - Escala de grises.
     - Espejar horizontal y vertical.
     - Negativo.
     - Comodín.
   - **Detalles:** No modifican metadatos ni requieren parámetros adicionales.

3. **`procesarImagenesFiltroArg0Met`:**
   - **Filtros:** rotar-izquirda, rotar-derecha.
   - **Detalles:** Modifican metadatos, pero no requieren parámetros.

4. **`procesarImagenesConcat`:**
   - **Filtros:**concatenar-vertical, concatenar-horizontal.
   - **Detalles:** Modifican metadatos y trabajan con dos imágenes simultáneamente.

5. **`procesarImagenesFiltroArg1`:**
   - **Filtros:**
     -aumentar-contraste
     -reducir-contraste
     -tonalidad-azul
     -tonalidad-roja
     -tonalidad-verde
   - **Detalles:** No modifican metadatos, pero requieren un porcentaje como parámetro.

- **Archivos de Configuración y Log**

Uso de Archivos

El programa permite el uso de archivos `.conf` para configurar los filtros y `.txt` para registrar el log de errores y flujo.

- **Archivos `.conf`:** Declarados con `--filtros=<archivo>`.
  - Se seleccionará el primer archivo válido y existente en el orden de argumentos.
  - Ejemplo:  
    Si `hola.conf` existe y `conf.conf` no, se usará `hola.conf`.

- **Archivos `.txt`:** Declarados con `--errores=<archivo>`.
  - Se seleccionará el primer archivo válido que:
    1. No tenga el mismo nombre que el archivo `.conf`.
    2. Tenga extensión `.txt`.

**Registro en el Archivo `.txt`**

El archivo `.txt` creado contiene:
- **Errores relevantes:**
  - Errores de memoria dinámica.
  - Porcentajes inválidos.
  - Argumentos incorrectos.
- **Éxitos:**
  - Imágenes generadas correctamente.
  - Eventos relevantes.

**Nota:** Los mensajes antes mostrados en pantalla se registran en el archivo `.txt` para facilitar el seguimiento del proceso.

---

## Notas Adicionales

- El archivo `.conf` debe existir previamente.
- El archivo `.txt` se crea automáticamente.
- Si hay múltiples archivos válidos para `.conf`, siempre se usará el primero en la lista.

---

## 🛠️ ¿Cómo usar?

Este programa es una **aplicación de consola**. Podés ejecutarlo desde una terminal o desde `cmd`, posicionándote en la carpeta donde se encuentra el ejecutable `bmpmanipuleitor.exe`.

### ▶️ Ejecución básica

```bash
bmpmanipuleitor.exe imagenDePrueba.bmp
```

> 📌 **Importante**: La imagen debe estar en formato **.bmp de 24 bits**. Si no agregás filtros, el programa no hará nada.

### 🎛️ Argumentos disponibles

#### ✅ Filtros que **no requieren valores**:

```
--escala-de-grises
--espejar-horizontal
--espejar-vertical
--rotar-derecha
--rotar-izquierda
--negativo
--comodin
```

#### 🔢 Filtros que **requieren valores (1–100)**:

```
--aumentar-contraste=valor
--reducir-contraste=valor
--tonalidad-azul=valor
--tonalidad-verde=valor
--tonalidad-roja=valor
--recortar=valor
--achicar=valor
```

> 📌 Por ejemplo: `--aumentar-contraste=50` aumentará el contraste en un 50% respecto de la imagen original.

#### 🖼️ Filtros que **requieren dos imágenes cargadas**:

```
--concatenar-horizontal
--concatenar-vertical
```

> Si las imágenes tienen distinta resolución, los píxeles sobrantes se rellenan con un color **dorado**.

---

### 🧪 Ejemplo de uso

```bash
bmpmanipuleitor.exe imagen1.bmp imagen2.bmp --escala-de-grises --reducir-contraste=30 --concatenar-vertical
```

Este comando carga dos imágenes, les aplica los filtros especificados y concatena el resultado verticalmente.

Podés cargar **tantas imágenes como quieras** y los argumentos **no necesitan estar en orden**.

---

### 📄 Uso de archivos externos

Podés aplicar filtros desde un archivo `.conf`, y generar un archivo de log con los eventos del programa.

```bash
--filtros=archivo.conf
--errores=log.txt
```

- El archivo `.conf` **debe existir**.
- El archivo de log `.txt` **se creará** si no existe.

#### Ejemplo completo

```bash
bmpmanipuleitor.exe imagenDePrueba1.bmp imagenDePrueba2.bmp --escala-de-grises --aumentar-contraste=30 --recortar=30 --concatenar-horizontal --filtros=filtros.conf --errores=log.txt
```

Este comando:
1. Carga las imágenes.
2. Aplica los filtros pasados por línea de comandos.
3. Aplica luego los filtros del archivo `.conf`.
4. Registra todo los eventos relacionados con la ejecucion de los filtros de fitros.conf en `log.txt`.

---

## 📦 Archivos incluidos

- Todos los archivos `.c` y `.h` necesarios para compilar el programa.
- El archivo del proyecto `Code::Blocks (.cbp)` para compilar fácilmente.
- Una carpeta `bin/Debug/` con el **ejecutable** precompilado (`.exe`).
- Imágenes de prueba listas para testear el programa rápidamente.

