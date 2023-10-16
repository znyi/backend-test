import express from 'express';
import bodyParser from 'body-parser';
import cors from 'cors'
import { createRequire } from 'node:module'

//set up cpp addon
const require = createRequire(import.meta.url)
const CppAddon = require('./build/Release/CppAddon.node')

//set up web back end
const app = express()
const PORT = 5000

const corsOptions = {
    origin: 'http://localhost:3000',
    optionsSuccessStatus: 200 // some legacy browsers (IE11, various SmartTVs) choke on 204
}
app.use(cors(corsOptions))

app.use(bodyParser.urlencoded({extended: true}))
app.use(bodyParser.json())

app.get('/primes', (req, res)=> {
    const queryObject = {
        startRange : parseInt(req.query.startRange), 
        endRange : parseInt(req.query.endRange),
    } 
    const queryString = JSON.stringify(queryObject)
    const outputString = CppAddon.getPrimes(queryString)
    const outputJSON = JSON.parse(outputString)
    res.send(outputJSON)
})

app.listen(PORT, ()=>{
    console.log(`app listening at port ${PORT}.`)
})