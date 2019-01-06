module.exports = {
    staticFileGlobs: [
      'build/static/css/**.css',
      'build/static/js/**.js',
      'build/media/**.png',
      'build/favicon.ico'
    ],
    swFilePath: './build/service-worker.js',
    stripPrefix: 'build/',
    runtimeCaching: [{
      urlPattern: /.*/,
      handler: 'cacheFirst'
    }],
    handleFetch: true
  }
