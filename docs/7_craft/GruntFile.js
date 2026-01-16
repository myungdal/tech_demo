module['exports'] = function (a) {
    a['initConfig']({
        'includeSource': {
            'options': { 'basePath': 'app' },
            'myTarget': { 'files': { 'app/index.html': 'app/_index.html' } }
        },
        'execute': { 'target': { 'src': ['generateStats.js'] } },
        'watch': [
            {
                'files': [
                    'app/_index.html',
                    'app/**/*.js'
                ],
                'tasks': ['includeSource'],
                'options': { 'livereload': 0x2328 }
            },
            {
                'files': ['app/stats.xlsx'],
                'tasks': ['execute'],
                'options': { 'livereload': 0x2328 }
            }
        ]
    });
    a['loadNpmTasks']('grunt-include-source');
    a['loadNpmTasks']('grunt-contrib-watch');
    a['loadNpmTasks']('grunt-execute');
    a['registerTask']('default', [
        'includeSource',
        'execute',
        'watch'
    ]);
};