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

