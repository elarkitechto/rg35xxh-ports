#!/usr/bin/env python3
"""Generate the shared import table for the pinned Threes ARM64 IL2CPP/Unity ELFs."""
from __future__ import annotations

import re
import struct
import sys
from pathlib import Path



def parse_undefined_symbols(path: Path) -> list[str]:
    data = path.read_bytes()
    if data[:4] != b"\x7fELF" or data[4] != 2 or data[5] != 1:
        raise ValueError("expected little-endian ELF64 (AArch64) libunity.so")
    shoff = struct.unpack_from("<Q", data, 0x28)[0]
    shentsize, shnum = struct.unpack_from("<HH", data, 0x3A)
    sections = [struct.unpack_from("<IIQQQQIIQQ", data, shoff+i*shentsize) for i in range(shnum)]
    names: set[str] = set()
    for section in sections:
        _name, sh_type, _flags, _addr, offset, size, link, _info, _align, entsize = section
        if sh_type != 11 or not entsize:  # SHT_DYNSYM
            continue
        strings = sections[link]
        string_data = data[strings[4]:strings[4]+strings[5]]
        for pos in range(offset, offset+size, entsize):
            name_index, _info, _other, shndx, _value, _size = struct.unpack_from("<IBBHQQ", data, pos)
            if shndx != 0 or not name_index:
                continue
            end = string_data.find(b"\0", name_index)
            symbol = string_data[name_index:end].decode("utf-8", "replace").split("@", 1)[0]
            if symbol and re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", symbol):
                names.add(symbol)
    if not names:
        raise ValueError("no undefined dynamic symbols found")
    return sorted(names)


def emit(symbols: list[str], output: Path) -> None:
    lines = [
        "// Generated from the pinned Threes! 3.1 ARM64 libunity.so.",
        '#include "so_util.h"', "#include <stdio.h>", "#include <stdint.h>", "#include <errno.h>", "#include <dlfcn.h>", "#include <string.h>", "",
    ]
    unsafe = symbols
    for name in unsafe:
        lines.append(f'long stub_{name}(void) {{ static unsigned n; if (n++ < 4) fprintf(stderr, "[THREES-STUB] {name}\\n"); return 0; }}')
    lines.extend(["", "DynLibFunction dynlib_functions[] = {"])
    for name in symbols:
        target = f"(uintptr_t)&stub_{name}"
        lines.append(f'  {{"{name}", {target}}},')
    lines.extend([
        "};",
        "size_t dynlib_numfunctions = sizeof(dynlib_functions) / sizeof(dynlib_functions[0]);",
        "",
        "static void *resolve_bionic(const char *name) {",
        '  if (!strcmp(name, "setjmp")) return dlsym(RTLD_DEFAULT, "_setjmp");',
        '  if (!strcmp(name, "longjmp")) return dlsym(RTLD_DEFAULT, "_longjmp");',
        '  if (!strcmp(name, "__errno")) return dlsym(RTLD_DEFAULT, "__errno_location");',
        "  return dlsym(RTLD_DEFAULT, name);",
        "}",
        "void recon_fill_passthrough(void) {",
        '  dlopen("libz.so.1", RTLD_NOW | RTLD_GLOBAL);',
        '  dlopen("libm.so.6", RTLD_NOW | RTLD_GLOBAL);',
        "  size_t real = 0;",
        "  for (size_t i = 0; i < dynlib_numfunctions; ++i) {",
        "    void *p = resolve_bionic(dynlib_functions[i].symbol);",
        "    if (p) { dynlib_functions[i].func = (uintptr_t)p; ++real; }",
        '    else fprintf(stderr, "[THREES-IMPORT] Android shim required: %s\\n", dynlib_functions[i].symbol);',
        "  }",
        '  fprintf(stderr, "[THREES-IMPORT] host functions=%zu Android shims=%zu\\n", real, dynlib_numfunctions-real);',
        "}",
        "",
    ])
    output.write_text("\n".join(lines))
    print(f"generated {output}: {len(symbols)} symbols, {len(unsafe)} guarded stubs")


def main() -> int:
    if len(sys.argv) != 4:
        print(f"usage: {Path(sys.argv[0]).name} PINNED_LIBIL2CPP_SO PINNED_LIBUNITY_SO OUTPUT_C", file=sys.stderr)
        return 2
    try:
        symbols = sorted(set(parse_undefined_symbols(Path(sys.argv[1]))) |
                         set(parse_undefined_symbols(Path(sys.argv[2]))))
        emit(symbols, Path(sys.argv[3]))
    except (OSError, ValueError, struct.error) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
