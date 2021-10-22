/* eslint-disable require-jsdoc */

const ClientError = require('../../exceptions/ClientError');
const NotFoundError = require('../../exceptions/NotFoundError');
/** shortener service
     */
class ShortenerHandler {
    constructor(service, validator) {
        this._service = service;
        this._validator = validator;
        this.getIndexHandler = this.getIndexHandler.bind(this);
        this.getIndexDetailsHandler = this.getIndexDetailsHandler.bind(this);
        this.postIndexHandler = this.postIndexHandler.bind(this);
        this.deleteIndexHandler = this.deleteIndexHandler.bind(this);
        this.redirectIndexHandler = this.redirectIndexHandler.bind(this);
        this.getStationDetailsHandler =
         this.getStationDetailsHandler.bind(this);
    }
    /*
     * function handler
     * @param  {object} error
     */
    handleClientError(error, res) {
        if (error instanceof ClientError) {
            return res.status(error.statusCode).json({
                status: 'fail',
                message: error.message,
            });
        }

        if (error instanceof NotFoundError) {
            return res.status(error.statusCode).json({
                status: 'fail',
                message: error.message,
            });
        }

        // Server ERROR!
        return res.status(500).json({
            status: 'failed',
            message: 'internal server execption',
        });
    }
    /**
     * @param  {object} req
     * @param  {object} res
     */
    async postIndexHandler(req, res) {
        try {
            this._validator.validateShortPayload(req.body);
            const {uuid, webid = 'Desa Made'} = req.body;
            const {stationID, shortUrl,
                longUrl} = await this._service.registerStation({uuid, webid});
            return res.status(200).json({
                status: 'success',
                message: 'Stasiun berhasil diregister',
                data: {
                    stationID,
                    longUrl,
                    shortUrl,
                },
            });
        } catch (e) {
            console.log(e);
            this.handleClientError(e, res);
        }
    }

    async redirectIndexHandler(req, res) {
        try {
            const {urlId} = req.params;
            const station = await this._service.getStationById(urlId);
            return res.redirect(station.longUrl);
        } catch (e) {
            console.log(e);
            this.handleClientError(e, res);
        }
    }

    async getIndexDetailsHandler(req, res) {
        try {
            const {urlId} = req.params;
            const station = await this._service.getStationById(urlId);
            return res.status(200).json({
                status: 'success',
                data: {
                    station,
                },
            });
        } catch (e) {
            console.log(e);
            this.handleClientError(e, res);
        }
    }

    async getStationDetailsHandler(req, res) {
        try {
            const {uuid} = req.params;
            const station = await this._service.getStationByUUId(uuid);
            return res.status(200).json({
                status: 'success',
                data: {
                    station,
                },
            });
        } catch (e) {
            console.log(e);
            this.handleClientError(e, res);
        }
    }

    async getIndexHandler(req, res) {
        try {
            const stations = await this._service.getStations();
            return res.status(200).json({
                status: 'success',
                data: {
                    stations,
                },
            });
        } catch (e) {
            console.log(e);
            this.handleClientError(e, res);
        }
    }

    async deleteIndexHandler(req, res) {
        try {
            const {uuid} = req.params;
            await this._service.deleteStationByUUId(uuid);
            return res.status(200).json({
                status: 'success',
                message: 'Stasiun berhasil unregister.',
            });
        } catch (e) {
            console.log(e);
            this.handleClientError(e, res);
        }
    }
}

module.exports = ShortenerHandler;
