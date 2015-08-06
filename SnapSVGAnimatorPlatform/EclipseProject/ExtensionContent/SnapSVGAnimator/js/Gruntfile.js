module.exports = function(grunt) {
    
    var pkg = grunt.file.readJSON("package.json");


  // Project configuration.
    grunt.initConfig({
        pkg: pkg,
        uglify: {
            options: {
                banner: '/*! <%= pkg.name %> <%= grunt.template.today("yyyy-mm-dd") %> */\n'
            },
            build: {
                src: '<%= pkg.name %>.js',
                dest: '<%= pkg.name %>.min.js'
            }
        },
        concat: {
            options: {
                banner: grunt.file.read("./app/banner.js")
                    .replace(/@VERSION/, pkg.version),
                footer: grunt.file.read("./app/footer.js")
            },
            target: {
                dest: "<%= pkg.name %>.js",
                src: [
                    "./app/garbagePool.js",
                    "./app/bitmap.js",
                    "./app/text.js",
                    "./app/shape.js",
                    "./app/movieclip.js",
                    "./app/commands.js",
                    "./app/resourceManager.js",
                    "./app/component.js"
                ]
            }
        }
  });

  // Load the plugin that provides the "uglify" task.
  grunt.loadNpmTasks('grunt-contrib-concat');
  grunt.loadNpmTasks('grunt-contrib-uglify');

  // Default task(s).
  grunt.registerTask('default', ['concat', 'uglify']);

};
