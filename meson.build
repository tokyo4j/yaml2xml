project(
  'yaml2xml',
  'c',
  version: '0.1.0',
  license: 'GPL-2.0-only',
  default_options: [
    'c_std=c11',
    'warning_level=2',
  ],
)

yaml = dependency('yaml-0.1')
xml2 = dependency('libxml-2.0')

executable(
  meson.project_name(),
  sources: files('yaml2xml.c'),
  dependencies: [yaml],
  install: true,
)

executable(
  'xml2yaml',
  sources: files('xml2yaml.c'),
  dependencies: xml2,
  install: true,
)
