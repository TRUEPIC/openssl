project('openssl', 'c', version: '3.0.0-alpha4')

openssl_c_args = [
  '-DOPENSSL_NO_SECURE_MEMORY',
  '-DSTATIC_LEGACY',
'-DOPENSSL_USE_NODELETE','-DL_ENDIAN','-DOPENSSL_PIC',
'-DOPENSSLDIR="' + meson.current_build_dir() / 'openssl/ssl' +'"',
'-DENGINESDIR="' + meson.current_build_dir() / 'openssl/lib/engines-3' + '"',
'-DMODULESDIR="' + meson.current_build_dir() / 'openssl/lib/ossl-modules' + '"',
]

if meson.is_cross_build()
  openssl_c_args += meson.get_external_property('c_args')
endif

ARCH = host_machine.cpu_family()

cc = meson.get_compiler('c')
env = environment()

libcrypto_inc = include_directories('include/')

subdir('crypto')

libcrypto_lib = static_library('crypto',
  c_args: openssl_c_args,
  include_directories: libcrypto_inc,
  dependencies: [
    crypto_dep,
  ],
  pic: true
)

libcrypto_dep = declare_dependency(
  link_with: libcrypto_lib,
  include_directories: [
    libcrypto_inc,
  ],
)
