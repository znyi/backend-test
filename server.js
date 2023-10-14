import express from 'express';
import bodyParser from 'body-parser';
import cors from 'cors'
import { createRequire } from 'node:module'

//set up cpp addon
const require = createRequire(import.meta.url)
const AngelDevAddon = require('./build/Release/AngelDevAddon.node')

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


app.get('/', (req, res)=> {
    res.send('hello server')
})
app.get('/echo', (req, res)=> {
    res.send(req.body)
})
app.get('/user/random', (req, res)=> {
    fetch("https://jsonplaceholder.typicode.com/users")
    .then(res => res.json())
    .then(users => {
        const dataSize = users.length
        const randomIndex = Math.floor(Math.random() * dataSize)
        res.send(users[randomIndex])
    })
})

app.get('/primes', (req, res)=> {
    const queryObject = {
        startRange : parseInt(req.query.startRange), 
        endRange : parseInt(req.query.endRange),
    } 
    const queryString = JSON.stringify(queryObject)
    const outputString = AngelDevAddon.getPrimes(queryString)
    const outputJSON = JSON.parse(outputString)
    console.log(outputJSON)
    res.send(outputJSON)
})

app.listen(PORT, ()=>{
    console.log(`app listening at port ${PORT}.`)
})