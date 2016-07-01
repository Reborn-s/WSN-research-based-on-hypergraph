#ifndef HMETIS_H
#define HMETIS_H
#if !defined(__cdecl)
#define __cdecl
#endif

#ifdef __cplusplus
extern "C" {
#endif

void __cdecl
HMETIS_PartRecursive(int ncells, int nnets, int *cellwgt, int *netptr,
int *netind, int *netwgt, int nparts, int ufactor, int *options,
int *part, int *edgecut);

void __cdecl
HMETIS_PartKway(int ncells, int nnets, int *cellwgt, int *netptr,
int *netind, int *netwgt, int nparts, int ufactor, int *options,
int *part, int *edgecut);

#ifdef __cplusplus
}
#endif
#endif