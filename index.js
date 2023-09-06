//
// Copyright 2018-2020 Orange
//
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
const express = require('express');
var openapi = require('express-openapi');
const swaggerUi = require('swagger-ui-express');
var multer = require('multer');
var cors = require('cors')

var StatusService = require('./api/services/statusService');
var CameraService = require('./api/services/CameraService');
var ServoService = require('./api/services/ServoService');
var MLX90641Service = require('./api/services/MLX90641Service');
const app = express();
app.use(cors())

var config = require("./config");
var bodyParser = require('body-parser');


openapi.initialize({
    app,
    apiDoc: './api/api-doc.yml',
    consumesMiddleware: {
        'application/json': bodyParser.json(),
        'multipart/form-data': function (req, res, next) {
            console.log("Running middleware")
            multer().any()(req, res, function (err) {
                if (err) {
                    return next(err);
                }
                // Handle both single and multiple files
                const filesMap = req.files.reduce(
                    (acc, f) =>
                        Object.assign(acc, {
                            [f.fieldname]: (acc[f.fieldname] || []).concat(f),
                        }),
                    {}
                );
                Object.keys(filesMap).forEach((fieldname) => {
                    const files = filesMap[fieldname];
                    req.body[fieldname] = files.length > 1 ? files.map(() => '') : '';
                });
                return next();
            });
        }
    },
    dependencies: {
        statusService: StatusService,
        CameraService: CameraService,
        ServoService:ServoService,
        MLX90641Service: MLX90641Service
    },
    paths: './api/paths',
    securityHandlers: {
        XAuthToken: async function (req, scopes, definition) {
            if (req.headers.hasOwnProperty("x-auth-token")) {
                try {
                    /*var user = await keyrock.me(req.headers["x-auth-token"])
                    //console.log("Security handler : " + JSON.stringify(user))
                    if (user.hasOwnProperty("id")) {
                        req.headers['userid'] = user.id;
                    }*/
                } catch (error) {
                    console.log("Failed to check token " + error)
                }
            } else {
                console.log('No x-auth-token provided')
            }
            return true;
        }
    },
    errorMiddleware: function (err, req, res, next) {
        console.log(JSON.stringify(err, null, 4))
        if ((err.hasOwnProperty("status")) && (err.hasOwnProperty("message"))) {
            res.status(err.status).send(err.message);
        } else {
            res.status(500).send(err);
        }
    }
});
//test
var options = {
    swaggerOptions: {
        url: '/api/api-docs'
    }
}
app.use(express.static('UI/dist'))
app.use('/api-docs', swaggerUi.serve, swaggerUi.setup(null, options));
app.use('/.well-known/acme-challenge/KvWRm8IiQO-xUQsEKm5NUxn-4e6WDvU28vcRJCJXIcE', function(req, res) {
    var text="KvWRm8IiQO-xUQsEKm5NUxn-4e6WDvU28vcRJCJXIcE.9Pel9Grl3pr_htwQC2qf0p50t1vd0S3kMujE5AM_KXE";
    res.write(text)
    res.end()
})
try {
    console.log("Listen " + config.SatelliteListenPort + " " + config.SatelliteListenIP)
    app.listen(config.SatelliteListenPort, config.SatelliteListenIP);
} catch (error) {
    console.log(error);
}