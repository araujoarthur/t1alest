# GALlib

Implementação de uma lista dinâmica (lista com arranjo) genérica em C.

>[!IMPORTANT]
>É obrigatória a utilização do compilador GCC por conta do macro `__typeof__`.

## Operações

- [X] Criar a lista (`gal_create`)
- [X] Expansão condicional de capacidade (`__gal_requires_resize` e `__gal_expand`)
- [X] Adicionar elemento (`gal_append`)
- [X] Adicionar elemento genérico (`gml_append`)
- [X] Obter elemento em determinado índice (`gal_getn`)
- [X] Obter elemento genérico em determinado índice (`gml_getn`)
- [X] Adicionar elemento em determinado indice (`gal_insert_at`)
- [X] Adicionar elemento genérico em determinado indice (`gml_insert_at`).
- [X] Remover último elemento (`gal_pop`)
- [X] Remover último elemento genérico (`gml_pop`)
- [X] Definir valor de elemento em determinado índice (`gal_setn`)
- [X] Definir valor de elemento genérico em determinado índice (`gml_setn`)
- [X] Remover elemento em índice determinado (`gal_remove_at`)
- [X] Buscar indice de elemento (`gal_find`)
- [X] Buscar indice de elemento genérico (`gml_find`)
- [X] Buscar ocorrências de elemento (`gal_find_all`)
- [ ] Buscar ocorrências de elemento genérico (`gml_find_all`)
- [X] Buscar ocorrência de sequência de elementos (`gal_lookup`)
- [X] ~~[REMOVIDO] Buscar ocorrência de sequência de elementos genéricos (`gml_lookup`) // A busca já é genérica (em void*), não teria porque.~~
- [ ] Buscar ocorrências de sequência de elementos (`gal_lookup_all`)
- [X] ~~[REMOVIDO]Buscar ocorrências de sequência de elementos genéricos(`gml_lookup_all`) // A busca já é genérica (em void*), não teria porque.~~
